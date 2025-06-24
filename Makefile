CXX := g++
CXXFLAGS = -std=c++20
SFML_INCLUDE := -I libs/SFML/include
SFML_LIB := -L libs/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system

LIBS:= -lpsapi

INCLUDE := -I include

EXECUTABLE := build/test/main.exe
OBJECTS := obj/UserInterface.o obj/UIElements.o obj/DiagnosticInfo.o obj/Rectangle.o obj/Circle.o obj/Object.o obj/PhysicsMath.o 
OBJECTS += obj/Engine.o obj/Physics.o obj/Vector2.o
OBJECTS += obj/Command.o obj/ObjectFactory.o obj/AbstractBox.o obj/Quadtree.o 

$(EXECUTABLE): src/main.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(SFML_INCLUDE) $(INCLUDE) -I src $^ -o $@ $(SFML_LIB) $(LIBS)

obj/Command.o: src/Engine/Command.cpp include/Command.hpp obj/ObjectFactory.o obj/Engine.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/Engine.o: src/Engine/Engine.cpp include/Engine.hpp obj/Object.o obj/PhysicsMath.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/Physics.o: src/Engine/Physics.cpp include/Physics.hpp obj/Object.o obj/PhysicsMath.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/ObjectFactory.o: src/Engine/ObjectFactory.cpp include/ObjectFactory.hpp obj/Object.o obj/Circle.o obj/Rectangle.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/Quadtree.o: src/Math/Quadtree.cpp include/Quadtree.hpp obj/Object.o obj/Circle.o obj/Rectangle.o obj/AbstractBox.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/Circle.o: src/Objects/Circle.cpp include/Circle.hpp obj/Object.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/Rectangle.o: src/Objects/Rectangle.cpp include/Rectangle.hpp obj/Object.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/Object.o: src/Objects/Object.cpp include/Object.hpp obj/Vector2.o obj/AbstractBox.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@
	
obj/AbstractBox.o: src/Math/AbstractBox.cpp include/AbstractBox.hpp obj/Vector2.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@
	
obj/UserInterface.o: src/UserInterface/UserInterface.cpp include/UserInterface.hpp obj/UIElements.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/UIElements.o: src/UserInterface/UIElements.cpp include/UIElements.hpp obj/Vector2.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/PhysicsMath.o: src/Math/PhysicsMath.cpp include/PhysicsMath.hpp obj/Vector2.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/Vector2.o: src/Math/Vector2.cpp include/Vector2.hpp
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

obj/DiagnosticInfo.o: src/Engine/DiagnosticInfo.cpp include/DiagnosticInfo.hpp
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	del /Q build\test\main.exe obj\*.o
else
	rm -rf build/test/main.exe obj/*.o
endif