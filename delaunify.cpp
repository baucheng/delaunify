#include "Delaunay_psm.h"
#include <iostream>

int main()
{
    // Disable unnecessary synchronization to optimize read speed
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::vector<double> delaunay_vertices;
    int line_number = 1; // 1-based input line counting
    int error_count = 0;
    for (std::string line; std::getline(std::cin, line);)
    {
        std::stringstream line_stream(line);
        double x, y;
        if (line_stream >> x >> y)
        {
            // Input coordinates parsed successfully
            delaunay_vertices.push_back(x);
            delaunay_vertices.push_back(y);
        }
        else
        {
            error_count++;
            std::cerr << "Error: could not read input line " << line_number << " as coordinates: \"" << line << "\"" << std::endl;
        }
        line_number++;
    }
    
    if (error_count != 0)
    {
        std::cerr << "Warning: " << error_count << " input line(s) were not correctly read. Vertex indexing may be unpredictable." << std::endl;
    }
    
    GEO::initialize();
    GEO::index_t num_dimensions = 2;
    GEO::Delaunay_var triangulation = GEO::Delaunay::create(GEO::coord_index_t(num_dimensions), "BDEL2d");
    GEO::index_t num_points = delaunay_vertices.size() / num_dimensions;
    
    //Actually perform the triangulation
    triangulation->set_vertices(num_points, delaunay_vertices.data());
    
    if (triangulation->nb_cells() == 0)
    {
        std::cerr << "Warning: No triangles in output. Input may be degenerate." << std::endl;
    }
    
    for (GEO::index_t i = 0; i < triangulation->nb_cells(); i++)
    {
        GEO::index_t v_0 = triangulation->cell_vertex(i, 0);
        GEO::index_t v_1 = triangulation->cell_vertex(i, 1);
        GEO::index_t v_2 = triangulation->cell_vertex(i, 2);
        
        // Using \n rather than std::endl (i.e. not flushing all the time) gives much faster write speeds
        std::cout << v_0 << " " << v_1 << " " << v_2 << "\n";
    }
    std::cout << std::flush;
    
    return 0;
}
