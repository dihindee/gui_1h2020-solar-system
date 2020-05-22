# gui_1h2020-solar-system
Проект: Симулятор системы космических тел
![Скриншот](https://i.imgur.com/O4qy1KJ.png)
Возможности:
1) Добавление и удаление космических тел.
2) Для каждого тела возможно построение траектории движения
3) Для каждого тела возможна настройка таких параметров, как: название, радиус (влияет на массу), ускорение свободного падения на поверхности (влияет на массу), координаты на сцене, скорость, цвет, число шагов при построении траектории, тело, относительно которого корректируется траектория.
В проекте используется QAbstractListModel,для задания модели в QListView и QComboBox.
Графическая часть использует QGraphicsView, QGraphicsItem.
