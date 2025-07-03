# cpp-3d-zelenka

---

### Описание:

Простой 3D движок на С++17, импорт 3D моделей (3ds, dae) через XML сцену.  
- Загрузить XML сцену "File->Open"
- Сохранить сцену "File->Save As"
- Выход "File->Exit", Exp  
  
В GUI отображается перечень моделей, их позиция в сцене, а так же позиция камеры.
Клавишами W/S перемещается камера по вертикали, A/D по горизонтали, Q/Z изменение дистанции до targer камеры.

### Технологии:

- OpenGL c GLSL 3.3
- Qt5 для GUI
- Assimp
- TinyXML
- IL
- СMake

### Запуск проекта:

Обновите драйвер видеокарты.

##### Windows  
Сборка проекта СMake, с компилятором GCC.  
Дополнительные пакеты библиотек были загружены pacman через MSYS2:  
pacman -S mingw-w64-x86_64-qt5 mingw-w64-x86_64-assimp  
pacman -S mingw-w64-x86_64-devil mingw-w64-x86_64-tinyxml  
Скорректируйте в CMakeLists.txt переменные CMAKE_INCLUDE_PATH и CMAKE_LIBRARY_PATH.  
    
##### Linex (Ubuntu)
Сборка проекта СMake, с компилятором GCC.  
Дополнительные пакеты библиотек:  
sudo apt install mesa-utils libgl1-mesa-dev libglu1-mesa-dev  
sudo apt install qt5-default qtbase5-dev qtchooser qt5-qmake  
sudo apt install libtinyxml-dev libdevil-dev  
sudo apt install libassimp-dev libassimp5  
Скорректируйте в CMakeLists.txt переменные CMAKE_INCLUDE_PATH и CMAKE_LIBRARY_PATH.  




