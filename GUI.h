#ifndef GUI_H
#define GUI_H
#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QInputDialog>
#include <QMouseEvent>
#include <QMenu>
#include <QPointF>
#include <QMessageBox>

#include "DirectedGraph.h"
#include "UndirectedGraph.h"

class GraphVisualizer : public QMainWindow {
    // Q_OBJECT

public:
    GraphVisualizer(QWidget* parent = nullptr) : QMainWindow(parent) {
        // Установка главного виджета
        auto centralWidget = new QWidget(this);
        auto layout = new QVBoxLayout(centralWidget);

        // Область рисования
        scene = new QGraphicsScene(this);
        scene->setBackgroundBrush(QBrush(Qt::white));
        view = new QGraphicsView(scene, this);
        layout->addWidget(view);

        // Элементы управления
        auto controlsLayout = new QHBoxLayout();

        auto graphTypeLabel = new QLabel("Тип графа:", this);
        controlsLayout->addWidget(graphTypeLabel);

        graphTypeComboBox = new QComboBox(this);
        graphTypeComboBox->addItem("Ориентированный");
        graphTypeComboBox->addItem("Неориентированный");
        connect(graphTypeComboBox, &QComboBox::currentIndexChanged, this, &GraphVisualizer::onGraphTypeChanged);
        controlsLayout->addWidget(graphTypeComboBox);

        auto clearButton = new QPushButton("Очистить граф", this);
        connect(clearButton, &QPushButton::clicked, this, &GraphVisualizer::clearGraph);
        controlsLayout->addWidget(clearButton);

        auto methodButton = new QPushButton("Использовать метод графа", this);
        connect(methodButton, &QPushButton::clicked, this, &GraphVisualizer::useGraphMethod);
        controlsLayout->addWidget(methodButton);

        layout->addLayout(controlsLayout);
        setCentralWidget(centralWidget);

        // Создание графов
        directedGraph = new DirectedGraph<int>();
        undirectedGraph = new UndirectedGraph<int>();
        currentGraph = directedGraph;

        // Настройка мыши
        view->setMouseTracking(true);
        view->viewport()->installEventFilter(this);
    }

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (obj == view->viewport()) {
            if (event->type() == QEvent::MouseButtonPress) {
                auto mouseEvent = static_cast<QMouseEvent*>(event);
                if (mouseEvent->button() == Qt::LeftButton) {
                    QPointF pos = view->mapToScene(mouseEvent->pos());
                    showContextMenu(pos);
                }
            }
        }
        return QMainWindow::eventFilter(obj, event);
    }

private slots:
    void onGraphTypeChanged(int index) {
        currentGraph = (index == 0) ? static_cast<IGraph<int>*>(directedGraph) : static_cast<IGraph<int>*>(undirectedGraph);
        updateGraphVisualization();
    }

    void showContextMenu(const QPointF& pos) {
        QMenu contextMenu;
        QAction* addVertexAction = contextMenu.addAction("Добавить вершину");
        QAction* addEdgeAction = contextMenu.addAction("Добавить ребро");
        QAction* selectedAction = contextMenu.exec(view->mapToGlobal(view->mapFromScene(pos)));

        if (selectedAction == addVertexAction) {
            addVertex(pos);
        } else if (selectedAction == addEdgeAction) {
            addEdge(pos);
        }
    }

    void addVertex(const QPointF& pos) {
        bool ok;
        int vertexID = QInputDialog::getInt(this, "Добавить вершину", "Введите ID вершины:", 0, 0, 1000, 1, &ok);
        if (ok) {
            currentGraph->AddVertex(vertexID);
            vertexPositions[vertexID] = pos;
            updateGraphVisualization();
        }
    }

    void addEdge(const QPointF&) {
        bool ok1, ok2, ok3;
        int from = QInputDialog::getInt(this, "Добавить ребро", "Введите начальную вершину:", 0, 0, 1000, 1, &ok1);
        int to = QInputDialog::getInt(this, "Добавить ребро", "Введите конечную вершину:", 0, 0, 1000, 1, &ok2);
        int weight = QInputDialog::getInt(this, "Добавить ребро", "Введите вес ребра:", 1, 1, 1000, 1, &ok3);

        if (ok1 && ok2 && ok3) {
            currentGraph->AddEdge(from, to, weight);
            updateGraphVisualization();
        }
    }

    void clearGraph() {
        scene->clear();
        directedGraph = new DirectedGraph<int>();
        undirectedGraph = new UndirectedGraph<int>();
        currentGraph = directedGraph;
        vertexPositions.clear();
    }

    void useGraphMethod() {
        QStringList methods;
        methods << "Найти кратчайший путь" << "Найти компоненты связности" << "Найти минимальное остовное дерево";
        bool ok;
        QString selectedMethod = QInputDialog::getItem(this, "Выбор метода", "Выберите метод:", methods, 0, false, &ok);

        if (ok) {
            if (selectedMethod == "Найти кратчайший путь") {
                findShortestPath();
            } else if (selectedMethod == "Найти компоненты связности") {
                findConnectedComponents();
            } else if (selectedMethod == "Найти минимальное остовное дерево") {
                findMST();
            }
        }
    }

    void findShortestPath() {
        bool ok1, ok2;
        int from = QInputDialog::getInt(this, "Кратчайший путь", "Начальная вершина:", 0, 0, 1000, 1, &ok1);
        int to = QInputDialog::getInt(this, "Кратчайший путь", "Конечная вершина:", 0, 0, 1000, 1, &ok2);

        if (ok1 && ok2) {
            try {
                int distance = currentGraph->FindBestPath(from, to);
                QMessageBox::information(this, "Результат", "Кратчайший путь: " + QString::number(distance));
            } catch (const std::exception& e) {
                QMessageBox::warning(this, "Ошибка", e.what());
            }
        }
    }

    void findConnectedComponents() {
        try {
            auto components = currentGraph->FindConnectedComponents();
            QString result = "Компоненты связности:\n";
            for (size_t i = 0; i < components->GetLength(); ++i) {
                result += "{ ";
                for (size_t j = 0; j < components->Get(i)->GetLength(); ++j) {
                    result += QString::number(components->Get(i)->Get(j)) + " ";
                }
                result += "}\n";
            }
            QMessageBox::information(this, "Результат", result);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Ошибка", e.what());
        }
    }

    void findMST() {
        try {
            auto mst = currentGraph->FindMST();
            QString result = "Минимальное остовное дерево:\n";
            for (size_t i = 0; i < mst->GetLength(); ++i) {
                result += "(" + QString::number(mst->Get(i).first) + ", " + QString::number(mst->Get(i).second) + ")\n";
            }
            QMessageBox::information(this, "Результат", result);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Ошибка", e.what());
        }
    }

    void updateGraphVisualization() {
        scene->clear();

        // Отображение вершин
        for (auto it = vertexPositions.begin(); it != vertexPositions.end(); ++it) {
            int vertex = it.key();
            QPointF pos = it.value();

            QGraphicsEllipseItem* circle = scene->addEllipse(pos.x() - 15, pos.y() - 15, 30, 30, QPen(Qt::black), QBrush(Qt::lightGray));
            QGraphicsTextItem* text = scene->addText(QString::number(vertex));
            text->setPos(pos.x() - 10, pos.y() - 10);
        }


        // Отображение ребер
        auto vertices = currentGraph->GetVertices();
        for (size_t i = 0; i < vertices->GetLength(); ++i) {
            int from = vertices->Get(i);
            auto edges = currentGraph->GetEdges(from);

            for (size_t j = 0; j < edges->GetLength(); ++j) {
                int to = edges->Get(j).first;
                int weight = edges->Get(j).second;

                if (vertexPositions.contains(from) && vertexPositions.contains(to)) {
                    QPointF fromPos = vertexPositions[from];
                    QPointF toPos = vertexPositions[to];

                    QGraphicsLineItem* line = scene->addLine(QLineF(fromPos, toPos), QPen(Qt::black));
                    QGraphicsTextItem* weightText = scene->addText(QString::number(weight));
                    weightText->setPos((fromPos.x() + toPos.x()) / 2, (fromPos.y() + toPos.y()) / 2);
                }
            }
        }
    }


private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    QComboBox* graphTypeComboBox;

    DirectedGraph<int>* directedGraph;
    UndirectedGraph<int>* undirectedGraph;
    IGraph<int>* currentGraph;

    QMap<int, QPointF> vertexPositions;

};

#endif //GUI_H
