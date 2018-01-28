// ---------------------------------------------------------------------
//
// Copyright (C) 2003 - 2017 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------



// check the cells generated by the CylindricalManifold for an arbitrary
// orientation

#include "../tests.h"
#include <deal.II/base/quadrature_lib.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/manifold_lib.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_values.h>
#include <deal.II/fe/mapping_c1.h>



const double xy_angle = numbers::PI/6;

template <int dim>
Point<dim> rotate_by_xy_angle (const Point<dim> &p)
{
  return Point<dim> (p[0]*std::cos(xy_angle) - p[1]*std::sin(xy_angle),
                     p[0]*std::sin(xy_angle) + p[1]*std::cos(xy_angle),
                     p[2]);
}


template <int dim>
void check ()
{
  Triangulation<dim> triangulation;
  GridGenerator::cylinder (triangulation);

  GridTools::transform ((Point<dim> ( *)(const Point<dim> &))&rotate_by_xy_angle<dim>, triangulation);

  static const CylindricalManifold<dim> boundary (Point<dim>(std::cos(xy_angle), std::sin(xy_angle), 0),
                                                  Point<dim>());
  triangulation.set_manifold (0, boundary);
  triangulation.refine_global (2);

  for (typename Triangulation<dim>::active_cell_iterator
       cell = triangulation.begin_active();
       cell!=triangulation.end(); ++cell)
    for (unsigned int i=0; i<GeometryInfo<dim>::vertices_per_cell; ++i)
      deallog << cell->vertex(i) << std::endl;
}


int main ()
{
  initlog();

  check<3> ();
}



