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
Все пакеты библиотек были загружены pacman с MSYS2 (C:/Dev/MSYS2/mingw64). 
Скорректируйте в CMakeLists.txt переменные CMAKE_INCLUDE_PATH и CMAKE_LIBRARY_PATH под свои условия. 
    
##### Linex (Ubuntu)
Сборка проекта СMake, с компилятором GCC.  
Установите необходимые пакеты, скорректируйте в CMakeLists.txt переменные CMAKE_INCLUDE_PATH и CMAKE_LIBRARY_PATH. 




