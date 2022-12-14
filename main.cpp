// Simple script to call Alpha Wrapping.

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/alpha_wrap_3.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Real_timer.h>
#include <iostream>
#include <string>

namespace AW3 = CGAL::Alpha_wraps_3;
namespace PMP = CGAL::Polygon_mesh_processing;

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point_3 = K::Point_3;
using Mesh = CGAL::Surface_mesh<Point_3>;

int main(int argc, char** argv) {
  std::cout.precision(17);
  if (argc == 1) {
    std::cout << "Usage: [bin] <mesh_file> <output_dst>" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string filename = argv[1];

  std::cout << "Reading " << filename << "..." << std::endl;
  Mesh mesh;
  if (!CGAL::IO::read_polygon_mesh(filename, mesh)) {
    std::cerr << "Failed to read in input file." << std::endl;
    return EXIT_FAILURE;
  }

  if (is_empty(mesh) || !is_triangle_mesh(mesh)) {
    std::cerr << "Invalid input." << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Input: " << num_vertices(mesh) << \
    " vertices, " << num_faces(mesh) << " faces" << std::endl;

  // Compute the alpha and offset values
  const double relative_alpha = (argc > 2) ? std::stod(argv[2]) : 20.;
  const double relative_offset = (argc > 3) ? std::stod(argv[3]) : 600.;
  CGAL::Bbox_3 bbox = CGAL::Polygon_mesh_processing::bbox(mesh);
  const double diag_length = std::sqrt(CGAL::square(bbox.xmax() - bbox.xmin()) +
                                       CGAL::square(bbox.ymax() - bbox.ymin()) +
                                       CGAL::square(bbox.zmax() - bbox.zmin()));
  const double alpha = diag_length / relative_alpha;
  const double offset = diag_length / relative_offset;

  // Construct the wrap
  CGAL::Real_timer t;
  t.start();
  Mesh wrap;
  CGAL::alpha_wrap_3(mesh, alpha, offset, wrap);
  t.stop();

  std::cout << "Result: " << num_vertices(wrap) << " vertices, " << num_faces(wrap) << " faces" << std::endl;

  // TODO maybe iterate over alpha and offset in order to find mesh that has a smaller size.
  std::cout << "Took " << t.time() << " s." << std::endl;
  // Save the result
  std::string output_name;
  if (argc > 2) {
    output_name = std::string(filename);
  } else {
    std::string input_name = std::string(filename);
    input_name = input_name.substr(input_name.find_last_of("/") + 1, input_name.length() - 1);
    input_name = input_name.substr(0, input_name.find_last_of("."));
    output_name = input_name
                              + "_" + std::to_string(static_cast<int>(relative_alpha))
                              + "_" + std::to_string(static_cast<int>(relative_offset)) + ".obj";
  }
  std::cout << "Writing to " << output_name << std::endl;
  CGAL::IO::write_polygon_mesh(output_name, wrap, CGAL::parameters::stream_precision(17));
  return EXIT_SUCCESS;
}
