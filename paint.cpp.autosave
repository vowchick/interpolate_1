#include "window.h"

void Window::paint_func_united (double (Window::*func) (double, int), double (Window::*func1) (double))
{
    QPainter painter (this);

    QPen pen_black(Qt::black, 0, Qt::SolidLine);
    QPen pen_red(Qt::red, 0, Qt::SolidLine);
    QPen pen_blue (Qt::blue, 0, Qt::SolidLine);
    QPen pen_green (Qt::green, 0, Qt::SolidLine);
    painter.setPen (pen_black);
    vis_a = (a + b) / 2 - (b - a) / (2 * scale);
    vis_b = (a + b) / 2 + (b - a) / (2 * scale);
    double max_f = 0, max_fy = 0, min_fy = 0,
           max_1 = 0, max_y1 = 0, min_y1 = 0,
           max = 0, max_y = 0, min_y = 0,
           maximum_y = 0, minimum_y = 0;
    max_f = find_max (&Window::fff, &max_fy, &min_fy);
    perturbation = max_f / 10;
    if ((method == 0 || method == 2 || method == 3) && n < 128)
      max_1 = find_max (func1, &max_y1, &min_y1);
    if (method == 1 || method == 2 || method == 3)
      max = find_max_spline (func, &max_y, &min_y);
    if (method == 0 && n < 128)
      {
        maximum_y = my_max (max_fy, max_y1);
        minimum_y = my_min (min_fy, min_y1);
      }
    else if (method == 1)
      {
        maximum_y = my_max (max_fy, max_y);
        minimum_y = my_min (min_fy, min_y);
      }
    else if (method == 2)
      {
        if (n < 128)
        {
          maximum_y = my_max (max_fy, my_max (max_y, max_y1));
          minimum_y = my_min (min_fy, my_min (min_y, min_y1));
        }
        else
          {
            maximum_y = my_max (max_fy, max_y);
            minimum_y = my_min (min_fy, min_y);
          }
      }
    else if (method == 3)
      {
        if (n < 128)
          {
            maximum_y = my_max (max_y, max_y1);
            minimum_y = my_min (min_y, min_y1);
          }
        else
          {
            maximum_y = max_y;
            minimum_y = min_y;
          }
      }
    //painter.save ();
    //scaling (&painter, minimum_y, maximum_y);
    double delta_y = maximum_y - minimum_y;
    if (delta_y < 1e-15)
        delta_y = 1e-15;
    if (method != 3)
      {
        sub_paint_func (&Window::fff, &painter, maximum_y, delta_y);
      }
    if ((method == 0 || method == 2 || method == 3) && n < 128)
      {
        painter.setPen (pen_blue);
        sub_paint_func (func1, &painter, maximum_y, delta_y);
      }
    if (method == 1 || method == 2 || method == 3)
      {
        painter.setPen (pen_green);
        sub_paint_func_spline (func, a, &painter, maximum_y, delta_y);
      }
    painter.setPen (pen_red);
    QPoint x1 = coord_to_scr (0, minimum_y, maximum_y, delta_y),
           x2 = coord_to_scr (0, maximum_y, maximum_y, delta_y),
           x3 = coord_to_scr (vis_a, 0, maximum_y, delta_y),
           x4 = coord_to_scr (vis_b, 0, maximum_y, delta_y);
    painter.setPen (pen_red);
    painter.drawLine (x1.x(), x1.y(), x2.x(), x2.y());
    painter.drawLine (x1,
                      x2);
    painter.drawLine (x3,
                      x4);

    // restore previously saved Coordinate System
    //painter.restore ();
    paint_rest (&painter, max_1, max, max_f);
}
QPoint
Window::coord_to_scr (double x, double y, double max_y, double delta_y) const
{

  int wid = width ();
  int hei = height ();
  double x_screen = static_cast<double>((wid - 1)) * (x - vis_a) / (vis_b - vis_a);
  double y_screen = static_cast<double>((hei - 1)) * (max_y - y) / delta_y;
  return QPoint (static_cast<int>(x_screen), static_cast<int>(y_screen));
}
void Window::sub_paint_func (double (Window::*func)(double),
                             QPainter *painter, double max_y, double delta_y)
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, delta_x = (vis_b - vis_a) / width();
    x1 = vis_a;
    y1 = (this->*func) (x1);
    int w = width ();
    //for (x2 = x1 + delta_x; x2 <= vis_b; x2 += delta_x)
    for (int i = 1; i < w - 1 ; i++)
      {
        x2 = x1 + delta_x;
        y2 = (this->*func) (x2);
        if (sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) > 1e-15)
          painter->drawLine (coord_to_scr (x1, y1, max_y, delta_y),
                            coord_to_scr (x2, y2, max_y, delta_y));

        x1 = x2;
        y1 = y2;
      }
}
void Window::sub_paint_func_spline (double (Window::*func)(double, int), double a,
                     QPainter *painter, double max_y, double delta_y)
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, delta_x = (vis_b - vis_a) / (width()),
           delta_x1 = (this->b - this->a) / n;
    x1 = a;
    x1 = vis_a;
    int j = 0;
    while (a + delta_x1 * j <= x1)
        j++;
    j--;
    y1 = (this->*func) (x1, j);
    int w = width ();
    for (int i = 1; i < w - 1; i++)
      {
        x2 = x1 + delta_x;
        while (a + delta_x1 * j <= x2)
            j++;
        j--;
        y2 = (this->*func) (x2, j);
        if (x1 >= vis_a && x2 <= vis_b)
          if (sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) > 1e-15)
            painter->drawLine (coord_to_scr (x1, y1, max_y, delta_y),
                               coord_to_scr (x2, y2, max_y, delta_y));


        x1 = x2;
        y1 = y2;
      }
}
double Window::find_max (double (Window:: *func) (double), double *maxy, double *miny)
{
    double x1 = vis_a, y1, max_y, min_y, max = 0, w = width (),
            delta_x = (vis_b - vis_a) / w, delta_y;
    max_y = min_y = 0;
    for (int i = 0; i < w; i++, x1 += delta_x)
      {
        y1 = (this->*func) (x1);
        if (y1 < min_y)
          min_y = y1;
        if (y1 > max_y)
          max_y = y1;
      }
    if (fabs (max_y) > fabs (min_y))
        max = fabs (max_y);
    else
        max = fabs (min_y);
    delta_y = 0.01 * (max_y - min_y);
    min_y -= delta_y;
    max_y += delta_y;
    *maxy = max_y;
    *miny = min_y;
    return max;
}
double Window::find_max_spline (double (Window:: *func) (double, int), double *maxy, double *miny)
{
    double x1, y1, max_y, min_y, max = 0,
            delta_x = (vis_b - vis_a) / std::max(4000, n + 1), delta_x1 = (b - a) / (n),
            delta = delta_x, delta_y;
    max_y = min_y = 0;
    for (int i = 0; i < n; i++)
      {
        x1 = a + delta_x1 * i;
        for (int j = 0; j <= delta_x1 / delta; j++, x1 += delta)
          {
            if (x1 > vis_a && x1 < vis_b)
              {
                 y1 = (this->*func) (x1, i);
                 if (y1 < min_y)
                   min_y = y1;
                 if (y1 > max_y)
                   max_y = y1;
              }
          }
      }
    if (fabs (max_y) > fabs (min_y))
        max = fabs (max_y);
    else
        max = fabs (min_y);
    delta_y = 0.01 * (max_y - min_y);
    min_y -= delta_y;
    max_y += delta_y;
    *maxy = max_y;
    *miny = min_y;
    return max;
}
void Window::scaling (QPainter *painter, double min_y, double max_y)
{
    painter->translate (0.5 * width (), 0.5 * height ());
    painter->scale (width () / (vis_b - vis_a), -height () / (my_max((max_y - min_y), 1e-15)));
    painter->translate (-0.5 * (vis_a + vis_b), -0.5 * (min_y + max_y));
}
void Window::paint_rest (QPainter *painter, double max_1, double max_2, double max_f)
{
    QPen pen_black(Qt::black, 0, Qt::SolidLine);
    char text[200];
    char text1[200];
    char text2[200];
    if (method != 3)
      {
        sprintf (text, "Maximum is equal to: %e\n", max_f);
        printf ("For the initial function, max = %e\n", max_f);
        maxLabel[0]->setText(text);
        maxLabel[0]->show();
      }
    if ((method == 0 || method == 2 || method == 3) && n < 128)
      {
        sprintf (text1, "Maximum is equal to: %e\n", max_1);
        printf ("For the first method, max = %e\n", max_1);
        maxLabel[1]->setText(text1);
        maxLabel[1]->show();
      }
    if (method == 1 || method == 2 || method == 3)
      {
        sprintf (text2, "Maximum is equal to: %e\n", max_2);
        printf ("For the second method, max = %e\n", max_2);
        maxLabel[2]->setText(text2);
        maxLabel[2]->show();
      }
    painter->setPen (pen_black);
    char ss[100];
    sprintf (ss, "scale = %.2f, n = %d, perturbation = %.2f", scale, n, disturb);
    info->setText(ss);
}
