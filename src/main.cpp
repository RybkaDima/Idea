#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui-SFML.h>
#include <imgui.h>
#include <fstream>

// первое множество
static const int SET_1 = 0;
// второе множество
static const int SET_2 = 1;
// пересечение множеств
static const int SET_CROSSED = 2;
// разность множеств
static const int SET_SINGLE = 3;

// Ширина окна
static const int WINDOW_SIZE_X = 800;
// Высота окна
static const int WINDOW_SIZE_Y = 800;

// буфер, хранящий координаты последней добавленной вершины
int lastAddPosBuf[2] = {0, 0};

// буфер кол-ва случайных точек
int lastRandoCntBuf[1] = {10};

// путь к файлу вывода
static const char OUTPUT_PATH[255] = "C:/Users/ryabkade.24/CLionProjects/Idea/out.txt";
// путь к файлу ввода
static const char INPUT_PATH[255] = "C:/Users/ryabkade.24/CLionProjects/Idea/in.txt";

// Точка
struct Point {
    // положение
    sf::Vector2i pos;
    // номер множества
    int setNum;

    // конструктор
    Point(const sf::Vector2i &pos, int setNum) : pos(pos), setNum(setNum) {
    }

    // получить случайную точку
    static Point randomPoint() {
        return Point(sf::Vector2i(
                             rand() % WINDOW_SIZE_X,
                             rand() % WINDOW_SIZE_Y),
                     rand() % 2
        );
    }
};

// Треугольник
struct Triangle {
    // положение
    sf::Vector2i pos1;
    sf::Vector2i pos2;
    sf::Vector2i pos3;

    // конструктор
    Triangle(const sf::Vector2i &pos1_, const sf::Vector2i &pos2_, const sf::Vector2i &pos3_){
        pos1=pos1_;pos2=pos2_;pos3=pos3_;
    }
    Triangle(){
    }
    static Triangle randomTriangle() {
        return Triangle(sf::Vector2i(
                             rand() % WINDOW_SIZE_X,
                             rand() % WINDOW_SIZE_Y),
                        sf::Vector2i(
                                rand() % WINDOW_SIZE_X,
                                rand() % WINDOW_SIZE_Y),
                        sf::Vector2i(
                                rand() % WINDOW_SIZE_X,
                                rand() % WINDOW_SIZE_Y)
        );
    }

};
struct Angle {
    // положение
    sf::Vector2i pos1;
    sf::Vector2i pos2;
    sf::Vector2i pos3;

    // конструктор
    Angle(const sf::Vector2i &pos1_, const sf::Vector2i &pos2_, const sf::Vector2i &pos3_){
        pos1=pos1_;pos2=pos2_;pos3=pos3_;
    }
    static Angle randomAngle() {
        return Angle(sf::Vector2i(
                                rand() % WINDOW_SIZE_X,
                                rand() % WINDOW_SIZE_Y),
                        sf::Vector2i(
                                rand() % WINDOW_SIZE_X,
                                rand() % WINDOW_SIZE_Y),
                        sf::Vector2i(
                                rand() % WINDOW_SIZE_X,
                                rand() % WINDOW_SIZE_Y)
        );
    }

};
// динамический список точек
std::vector<Point> points;
std::vector<Triangle> triangles;
std::vector<Angle> angles;

// цвет фона
static sf::Color bgColor;
// значение цвета по умолчанию
float color[3] = {0.12f, 0.12f, 0.13f};

// задать цвет фона по вещественному массиву компонент
static void setColor(float *pDouble) {
    bgColor.r = static_cast<sf::Uint8>(pDouble[0] * 255.f);
    bgColor.g = static_cast<sf::Uint8>(pDouble[1] * 255.f);
    bgColor.b = static_cast<sf::Uint8>(pDouble[2] * 255.f);
}

// рисование параметров цвета
void ShowBackgroundSetting() {
    // если не раскрыт список `Background`
    if (!ImGui::CollapsingHeader("Background"))
        // заканчиваем выполнение
        return;

    // Инструмент выбора цвета
    if (ImGui::ColorEdit3("Background color", color)) {
        // код вызывается при изменении значения
        // задаём цвет фона
        setColor(color);
    }
    // конец рисование окна
}


// рисование задачи на невидимом окне во всё окно приложения
void RenderTask() {
    // задаём левый верхний край невидимого окна
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    // задаём правый нижний край невидимого окна
    ImGui::SetNextWindowSize(ImVec2(WINDOW_SIZE_X, WINDOW_SIZE_Y));
    // создаём невидимое окно
    ImGui::Begin("Overlay", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
    // получаем список примитивов, которые будут нарисованы
    auto pDrawList = ImGui::GetWindowDrawList();

    // перебираем точки из динамического массива точек
    for (auto point: points) {
        ImColor clr;
        // Устанавливаем цвет по номеру множества
        switch (point.setNum) {
            case SET_1:
                clr = ImColor(200, 100, 150);
                break;
            case SET_2:
                clr = ImColor(100, 200, 150);
                break;
            case SET_CROSSED:
                clr = ImColor(100, 150, 200);
                break;
            case SET_SINGLE:
                clr = ImColor(150, 200, 100);
                break;
        }
        // добавляем в список рисования круг
        pDrawList->AddCircleFilled(
                sf::Vector2i(point.pos.x, point.pos.y),
                3,
                clr,
                20
        );
    }
    for (auto triangle: triangles) {
        ImColor clr;
        // добавляем в список рисования круг
        pDrawList->AddCircleFilled(
                sf::Vector2i(triangle.pos1.x, triangle.pos1.y),
                3,
                clr,
                20
        );
        pDrawList->AddCircleFilled(
                sf::Vector2i(triangle.pos2.x, triangle.pos2.y),
                3,
                clr,
                20
        );
        pDrawList->AddCircleFilled(
                sf::Vector2i(triangle.pos3.x, triangle.pos3.y),
                3,
                clr,
                20
        );
        pDrawList->AddLine(
                sf::Vector2i(triangle.pos1.x, triangle.pos1.y),
                sf::Vector2i(triangle.pos2.x, triangle.pos2.y),
                3,
                clr
        );
        pDrawList->AddLine(
                sf::Vector2i(triangle.pos1.x, triangle.pos1.y),
                sf::Vector2i(triangle.pos3.x, triangle.pos3.y),
                3,
                clr
        );
        pDrawList->AddLine(
                sf::Vector2i(triangle.pos3.x, triangle.pos3.y),
                sf::Vector2i(triangle.pos2.x, triangle.pos2.y),
                3,
                clr
        );
    }
    // заканчиваем рисование окна
    ImGui::End();
}

// ручное добавление элементов
void ShowAddElem() {
    // если не раскрыта панель `Add Elem`
    if (!ImGui::CollapsingHeader("Add Elem"))
        // заканчиваем выполнение
        return;

    // Инструмент выбора цвета
    if (ImGui::DragInt2("Coords", lastAddPosBuf, 0.5f, 0, std::min(WINDOW_SIZE_X, WINDOW_SIZE_Y))) {
        // никаких действий не требуется, достаточно
        // тех изменений буфера, которые imGui выполняет
        // автоматически
    }

    // фиксируем id равный 0 для первого элемента
    ImGui::PushID(0);
    // если нажата кнопка `Set 1`
    if (ImGui::Button("Set 1"))
        // добавляем то добавляем в список точку, принадлежащую первому множеству
        points.emplace_back(Point(sf::Vector2i(lastAddPosBuf[0], lastAddPosBuf[1]), SET_1));
    // восстанавливаем буфер id
    ImGui::PopID();

    // говорим imGui, что следующий элемент нужно рисовать на той же линии
    ImGui::SameLine();
    // задаём id, равный одному
    ImGui::PushID(1);
    // если нажата кнопка `Set 2`
    if (ImGui::Button("Set 2"))
        // добавляем то добавляем в список точку, принадлежащую второму множеству
        points.emplace_back(Point(sf::Vector2i(lastAddPosBuf[0], lastAddPosBuf[1]), SET_2));
    // восстанавливаем буфер id
    ImGui::PopID();
}

// добавить заданное кол-во случайных точек
void randomize(int cnt) {
    for (int i = 0; i < cnt; i++) {
        points.emplace_back(Point::randomPoint());
    }
}

// панель добавления случайных точек
void ShowRandomize() {
    // если не раскрыта панель `Randomize`
    if (!ImGui::CollapsingHeader("Randomize"))
        // заканчиваем выполнение
        return;

    // первый элемент в строке
    ImGui::PushID(0);

    // Инструмент выбора кол-ва
    if (ImGui::DragInt("Count", lastRandoCntBuf, 0.1, 0, 100)) {

    }
    // восстанавливаем буффер id
    ImGui::PopID();
    // следующий элемент будет на той же строчке
    ImGui::SameLine();
    // второй элемент
    ImGui::PushID(1);
    // создаём кнопку добавления
    if (ImGui::Button("Add"))
        // по клику добавляем заданное число случайных точек
        randomize(lastRandoCntBuf[0]);
    ImGui::PopID();
}

// загрузка из файла
void loadFromFile() {
    // открываем поток данных для чтения из файла
    std::ifstream input(INPUT_PATH);
    // очищаем массив точек
    points.clear();
    // пока не достигнут конец файла
    while (!input.eof()) {
        int x, y, s;
        input >> x; // читаем x координату
        input >> y; // читаем y координату
        input >> s; // читаем номер множества
        // добавляем в динамический массив точку на основе прочитанных данных
        points.emplace_back(Point(sf::Vector2i(x, y), s));
    }
    // закрываем файл
    input.close();
}

// запись в файл
void saveToFile() {
    // открываем поток данных для записи в файл
    std::ofstream output(OUTPUT_PATH);

    // перебираем точки
    for (auto point: points) {
        // выводим через пробел построчно: x-координату, y-координату и номер множества
        output << point.pos.x << " " << point.pos.y << " " << point.setNum << std::endl;
    }

    // закрываем
    output.close();
}

// работа с файлами
void ShowFiles() {
    // если не раскрыта панель `Files`
    if (!ImGui::CollapsingHeader("Files"))
        // заканчиваем выполнение
        return;

    // первый элемент в линии
    ImGui::PushID(0);
    // создаём кнопку загрузки
    if (ImGui::Button("Load")) {
        // загружаем данные из файла
        loadFromFile();
    }
    // восстанавливаем буфер id
    ImGui::PopID();

    // следующий элемент будет на той же строчке
    ImGui::SameLine();
    // второй элемент
    ImGui::PushID(1);
    // создаём кнопку сохранения
    if (ImGui::Button("Save")) {
        // сохраняем задачу в файл
        saveToFile();
    }
    // восстанавливаем буфер id
    ImGui::PopID();
}

// решение задачи
void solve() {
    // у совпадающих по координатам точек меняем множество на SET_CROSSED
    for (int i = 0; i < points.size(); i++)
        for (int j = i + 1; j < points.size(); j++)
            if (points[i].pos == points[j].pos)
                points[i].setNum = points[j].setNum = SET_CROSSED;

    // у всех точек, у которых множество не SET_CROSSED, задаём множество SET_SINGLE
    for (auto &point: points)
        if (point.setNum != SET_CROSSED)
            point.setNum = SET_SINGLE;

}

// решение задачи
void ShowSolve() {
    // если не раскрыта панель `Solve`
    if (!ImGui::CollapsingHeader("Solve"))
        return;
    // первый элемент в линии
    ImGui::PushID(0);
    // создаём кнопку решения
    if (ImGui::Button("Solve")) {
        solve();
    }

    // восстанавливаем буфер id
    ImGui::PopID();

    // следующий элемент будет на той же строчке
    ImGui::SameLine();
    // второй элемент
    ImGui::PushID(1);

    // создаём кнопку очистки
    if (ImGui::Button("Clear")) {
        // удаляем все точки
        points.clear();
    }
    // восстанавливаем буфер id
    ImGui::PopID();
}

// помощь
void ShowHelp() {
    if (!ImGui::CollapsingHeader("Help"))
        return;

    // первый заголовок
    ImGui::Text("ABOUT THIS DEMO:");
    // первый элемент списка
    ImGui::BulletText("Author Ivanov Michail 17-1");
    // второй элемент списка
    ImGui::BulletText("Powered by SFML+ImGui");
    // разделитель
    ImGui::Separator();

    // второй заголовок
    ImGui::Text("TASK:");
    // первый элемент списка(многострочный)
    ImGui::BulletText("Two sets of points are given\n"
                      "in an integer two-dimensional space.\n"
                      "It is required to build intersections and\n"
                      "the difference between these sets.");
    // разделитель
    ImGui::Separator();
}

// главный метод
int main() {
    // создаём окно для рисования
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Geometry Project 10");
    // задаём частоту перерисовки окна
    window.setFramerateLimit(60);
    // инициализация imgui+sfml
    ImGui::SFML::Init(window);

    // задаём цвет фона
    setColor(color);

    // переменная таймера
    sf::Clock deltaClock;
    // пока окно открыто, запускаем бесконечный цикл
    Triangle triangle;
    triangle.randomTriangle();
    triangles.push_back(triangle);
    while (window.isOpen()) {
        // создаём событие sfml
        sf::Event event;
        // пока окно принимает события
        while (window.pollEvent(event)) {
            // отправляем события на обработку sfml
            ImGui::SFML::ProcessEvent(event);

            // если событие - это закрытие окна
            if (event.type == sf::Event::Closed) {
                // закрываем окно
                window.close();
            }
            // если событие - это клик мышью
            if (event.type == sf::Event::MouseButtonPressed) {
                // если мышь не обрабатывается элементами imGui
                if (!ImGui::GetIO().WantCaptureMouse) {
                    // меняем координаты последней добавленной точки
                    lastAddPosBuf[0] = event.mouseButton.x;
                    lastAddPosBuf[1] = event.mouseButton.y;
                    // если левая кнопка мыши
                    if (event.mouseButton.button == sf::Mouse::Button::Left)
                        points.emplace_back(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), SET_1);
                    else
                        points.emplace_back(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), SET_2);
                }
            }
        }

        // запускаем обновление окна по таймеру с заданной частотой
        ImGui::SFML::Update(window, deltaClock.restart());

        // рисование задания
        RenderTask();

        // делаем окно полупрозрачным
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.13f, 0.8f));

        // создаём окно управления
        ImGui::Begin("Control");

        // рисование параметров цвета
        ShowBackgroundSetting();
        // ручное добавление элементов
        ShowAddElem();

        // добавление случайных точек
        ShowRandomize();

        // работа с файлами
        ShowFiles();

        // решение задачи
        ShowSolve();

        // помощь
        ShowHelp();

        // конец рисования окна
        ImGui::End();

        // Возвращаем цвет окна к исходному
        ImGui::PopStyleColor();

        // очищаем окно
        window.clear(bgColor);
        // рисуем по окну средствами imgui+sfml
        ImGui::SFML::Render(window);
        // отображаем изменения на окне
        window.display();
    }

    // завершаем работу imgui+sfml
    ImGui::SFML::Shutdown();

    return 0;
}