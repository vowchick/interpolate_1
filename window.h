
#ifndef WINDOW_H
#define WINDOW_H

#include "methods.h"
#include <QtWidgets/QtWidgets>
#include <memory>
#include <QPushButton>
class Window : public QWidget
{
  Q_OBJECT

private:
  const char *f_name;
  double a;
  double b;
  double vis_a;
  double vis_b;
  QPushButton *PushButton;
  QPushButton *PushButton1;
  QLabel *currentMethod;
  QLabel *info;
  QLabel *maxLabel[3];
  QLabel *func_name;
  int n;
  int k;
  double (*f) (double);
  double scale;
  int method;
  double perturbation;
  double disturb;
  std::unique_ptr<double[]> d_i;
  std::unique_ptr<double[]> diff;
  std::unique_ptr<double[]> diff1;
  std::unique_ptr<double[]> x;
  std::unique_ptr<double[]> C2;
  std::unique_ptr<double[]> F;
  QString methodString[4] = {"Current method: Newton and Original",
                             "Current method: Splines and Original",
                             "Current method: Newton and Splines and Original",
                             "Current method: Residuals"};
public:
  Window (QWidget *parent);
  ~Window ();
  QSize minimumSizeHint () const;
  QSize sizeHint () const;
  QPoint
  coord_to_scr (double x, double y, double max_y, double delta_y) const;
  void prepare ();
  double ff (double x);
  double fff (double x);
  double ff1 (double x, int i);
  double find_max (double (Window:: *func) (double), double *maxy, double *miny);
  double find_max_spline (double (Window:: *func) (double, int), double *maxy, double *miny);
  void scaling (QPainter *painter, double min_y, double max_y);
  void paint_rest(QPainter *painter, double max_1, double max_2, double max_f);
  void paint_func (double (Window::*func) (double));
  void recalculate ();
  void paint_func_spline (double (Window::*func) (double, int));
  void paint_func_united (double (Window::*func) (double, int), double (Window::*func2) (double));
  void sub_paint_func (double (Window::*func)(double),
                       QPainter *painter, double max_y, double delta_y);
  void sub_paint_func_spline (double (Window::*func)(double, int), double a,
                       QPainter *painter, double max_y, double delta_y);
  int parse_command_line (int argc, char *argv[]);

public slots:
  void change_func ();
  void double_n ();
  void undouble_n ();
  void change_method ();
  void increase ();
  void decrease ();
  void plus ();
  void minus ();
private slots:
  void button_plus_pressed ();
  void button_minus_pressed ();

protected:
  void paintEvent (QPaintEvent *event);
};

#endif
