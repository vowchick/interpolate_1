
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include "window.h"

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  QMainWindow *window = new QMainWindow;
  QMenuBar *tool_bar = new QMenuBar (window);
  Window *graph_area = new Window (window);
  QAction *action;

  if (graph_area->parse_command_line (argc, argv))
    {
      QMessageBox::warning (0, "Wrong input arguments!", 
                            "Wrong input arguments!");
      printf ("usage : ./a.out a b n k\n");
      return -1;
    }
  graph_area->change_func();
  action = tool_bar->addAction ("&Change function", graph_area, SLOT (change_func ()));
  action->setShortcut (QString ("0"));


  action = tool_bar->addAction ("&Double n", graph_area, SLOT (double_n ()));
  action->setShortcut (QString ("4"));

  action = tool_bar->addAction ("&Undouble n", graph_area, SLOT (undouble_n ()));
  action->setShortcut (QString ("5"));

  action = tool_bar->addAction ("&Change method", graph_area, SLOT (change_method ()));
  action->setShortcut (QString ("1"));

  action = tool_bar->addAction ("&Increase", graph_area, SLOT (increase ()));
  action->setShortcut (QString ("2"));

  action = tool_bar->addAction ("&Decrease", graph_area, SLOT (decrease ()));
  action->setShortcut (QString ("3"));

  action = tool_bar->addAction ("&Positive Perturbation", graph_area, SLOT (plus ()));
  action->setShortcut (QString ("6"));

  action = tool_bar->addAction ("&Negative Perturbation", graph_area, SLOT (minus ()));
  action->setShortcut (QString ("7"));

  action = tool_bar->addAction ("&Exit", window, SLOT (close ()));
  action->setShortcut (QString ("Ctrl+X"));

  tool_bar->setMaximumHeight (30);

  window->setMenuBar (tool_bar);
  window->setCentralWidget (graph_area);
  window->setWindowTitle ("Graph");
  graph_area->recalculate();
  window->show ();
  app.exec ();
  delete window;
  return 0;
}
