#include <iostream>
#include "Test.h"
#include "GUI.h"
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    GraphVisualizer visualizer;
    visualizer.show();
    return app.exec();
}
