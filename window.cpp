
#include <QPainter>
#include <stdio.h>

#include "window.h"
#include <math.h>

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 9

static
double f_0 (double x)
{
    return 1 + x * 0;
}

static
double f_1 (double x)
{
    return x;
}

static
double f_2 (double x)
{
    return x * x;
}

static
double f_3 (double x)
{
    return x * x * x;
}

static
double f_4 (double x)
{
    return x * x * x * x;
}

static
double f_5 (double x)
{
    return exp (x);
}

static
double f_6 (double x)
{
    return 1. / (25. * x * x + 1.);
}
Window::Window (QWidget *parent)
    : QWidget (parent)
{
    a = DEFAULT_A;
    b = DEFAULT_B;
    n = DEFAULT_N;
    scale = 1;
    method = 0;
    disturb = 0;
    perturbation = 0;
    PushButton = new QPushButton (this);
    PushButton->setGeometry (50, 50, 60, 60);
    PushButton->setText("+");
    PushButton1 = new QPushButton (this);
    PushButton1->setGeometry (120, 50, 60, 60);
    PushButton1->setText("-");
    QVBoxLayout *layout = new QVBoxLayout (this);
    QHBoxLayout *scale_layout = new QHBoxLayout ();

    scale_layout->addWidget (PushButton);
    scale_layout->addWidget(PushButton1);
    scale_layout->setAlignment(Qt::AlignLeft);

    func_name = new QLabel (this);
    func_name->setFixedWidth(300);
    func_name->setFrameStyle(QFrame::Panel);
    maxLabel[0] = new QLabel (this);
    maxLabel[0]->setFixedWidth(300);
    maxLabel[0]->setFrameStyle(QFrame::Panel);
    maxLabel[1] = new QLabel (this);
    maxLabel[1]->setFixedWidth(300);
    maxLabel[1]->setFrameStyle(QFrame::Panel);
    maxLabel[2] = new QLabel (this);
    maxLabel[2]->setFixedWidth(300);
    maxLabel[2]->setFrameStyle(QFrame::Panel);
    currentMethod = new QLabel (this);
    info =  new QLabel (this);
    currentMethod->setText (methodString[0]);
    maxLabel[0]->setStyleSheet("QLabel {color : black; }");
    maxLabel[1]->setStyleSheet("QLabel {color : blue; }");
    maxLabel[2]->setStyleSheet("QLabel {color : green; }");
    maxLabel[0]->hide();
    maxLabel[1]->hide();
    maxLabel[2]->hide();


    layout->addWidget (func_name);
    layout->addWidget (maxLabel[0]);
    layout->addWidget (maxLabel[1]);
    layout->addWidget (maxLabel[2]);
    layout->addWidget(currentMethod);
    layout->addWidget(info);
    layout->addLayout (scale_layout);
    layout->setAlignment(Qt::AlignLeft);
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);
    connect (PushButton1, SIGNAL (clicked ()), this,
             SLOT (button_minus_pressed ()),
             Qt:: UniqueConnection);
    connect (PushButton, SIGNAL (clicked ()), this,
             SLOT (button_plus_pressed ()),
             Qt:: UniqueConnection);
}
Window::~Window ()
{
    delete PushButton;
    delete PushButton1;
}
QSize Window::minimumSizeHint () const
{
    return QSize (100, 100);
}

QSize Window::sizeHint () const
{
    return QSize (1000, 1000);
}
void Window::button_plus_pressed ()
{
    scale *= 2;
    update ();
}
void Window::button_minus_pressed ()
{
    if (scale >= 2)
        scale /= 2;
    else
        printf ("s is to small\n");
    update ();
}
int Window::parse_command_line (int argc, char *argv[])
{
    if (argc == 1)
        return -1;

    if (argc == 2)
        return -1;
    if (argc == 3)
        return -1;
    if (argc == 4)
        return 0;
    if (   sscanf (argv[1], "%lf", &a) != 1
           || sscanf (argv[2], "%lf", &b) != 1
           || b - a < 1.e-6
           || sscanf (argv[3], "%d", &n) != 1
           || n <= 0
           || (argc > 4 && sscanf (argv[4], "%d", &k) != 1)
           || k < 0)
        return -3;
    char ss[40];
    sprintf (ss, "scale = %.2f, n = %d perturbation = %.2f", scale, n, disturb);
    info->setText(ss);
    func_name->setText(f_name);
    return 0;
}
void Window::increase ()
{
    scale *= 2;
    update ();
}
void Window::decrease ()
{
    if (scale >= 2)
        scale /= 2;
    else
        printf ("s is to small\n");
    update ();
}
/// change current function for drawing
void Window::change_func ()
{
    switch (k)
    {
    case 0:
        f_name = "f (x) = 1";
        f = f_0;
        break;
    case 1:
        f_name = "f (x) = x";
        f = f_1;
        break;
    case 2:
        f_name = "f (x) = x^2";
        f = f_2;
        break;
    case 3:
        f_name = "f (x) = x^3";
        f = f_3;
        break;
    case 4:
        f_name = "f (x) = x^4";
        f = f_4;
        break;
    case 5:
        f_name = "f (x) = e^x";
        f = f_5;
        break;
    case 6:
        f_name = "f (x) = 1/(25x^2 + 1)";
        f = f_6;
        break;
    }
    func_name->setText(f_name);
    k = (k + 1) % 7;
    recalculate();
    update ();
}
void Window::double_n()
{
    if (method == 0)
    {
        if (n * 2 >= 128)
            printf ("n is to big\n");
        else
            n *= 2;
    }
    else
    {
        n *= 2;
    }
    recalculate();
    update ();
}
void Window::undouble_n()
{
    if (n < 2)
        printf ("n is to small\n");
    else
        n /= 2;
    recalculate();
    update ();
}
void Window::change_method()
{
    method += 1;
    method = method % 4;
    if (method == 0 && n >= 128)
    {
        printf ("n was to big and now it's 128\n");
        n = 64;
    }
    currentMethod->setText(methodString[method]);
    recalculate();
    update ();
}
/// render graph
void Window::paintEvent (QPaintEvent * /* event */)
{
    paint_func_united (&Window::ff1, &Window::ff);
}
void Window::prepare ()
{
    double x1 = 0, delta_x = (b - a) / n;
    double i = 0;
    for (x1 = a, i = 0; i <= n; x1 += delta_x, i++)
    {
        F[i] = f (x1);
        x[i] = x1;
    }
    F[n >> 1] += disturb;
}
double Window::ff (double x)
{
    if (method != 3)
        return Pf(x, n, a, b, diff.get ());
    else
    {
        return fabs (Pf(x, n, a, b, diff.get ()) - f (x));
    }
}
double Window:: fff (double x)
{
    return f (x);
}
double Window::ff1 (double x, int i)
{
    if (method != 3)
        return Pf(x, n, a, b, C2.get (), i);
    else
    {
        return fabs (Pf(x, n, a, b, C2.get (), i) - f (x));
    }
}
void Window::plus()
{
    disturb += perturbation;
    recalculate();
    update ();
}
void Window::minus()
{
    disturb -= perturbation;
    recalculate();
    update ();
}
void Window::recalculate()
{
    x = std::make_unique<double[]>(n + 1);
    F = std::make_unique<double[]>(n + 1);
    diff = std::make_unique<double[]>(n + 1);
    diff1 = std::make_unique<double[]>(n + 1);
    prepare();
    if (method == 0 || method == 2 || method == 3)
    {
        if (n < 128)
            first_method (n, x.get(), F.get(), diff.get());
        //paint_func (&Window::ff);
    }
    if (method == 1 || method == 2 || method == 3)
    {
        C2 = std::make_unique<double[]>(4 * n + 4);
        second_method(n, x.get (), F.get (), diff1.get (), C2.get ());
        //paint_func_spline (&Window::ff1);
    }
        maxLabel[0]->hide();
        maxLabel[1]->hide();
        maxLabel[2]->hide();
}
