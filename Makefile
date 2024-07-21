CXX := g++
SFML_INCLUDE := -I libs/SFML/include
SFML_LIB := -L libs/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system

EXECUTABLE := build/test/main.exe
OBJECTS := src/Objects/obj/Rectangle.o src/Objects/obj/Circle.o src/Objects/obj/Object.o src/Math/obj/PhysicsMath.o 
OBJECTS += src/Engine/obj/Engine.o src/Engine/obj/Physics.o

$(EXECUTABLE): src/main.cpp $(OBJECTS)
	$(CXX) $(SFML_INCLUDE) -I src $^ -o $@ $(SFML_LIB)

src/Engine/obj/Engine.o: src/Engine/Engine.cpp src/Objects/obj/Object.o src/Math/obj/PhysicsMath.o
	$(CXX) $(SFML_INCLUDE) -I src -c $< -o $@

src/Engine/obj/Physics.o: src/Engine/Physics.cpp src/Objects/obj/Object.o src/Math/obj/PhysicsMath.o
	$(CXX) $(SFML_INCLUDE) -I src -c $< -o $@

src/Math/obj/PhysicsMath.o: src/Math/PhysicsMath.cpp src/Objects/obj/Object.o
	$(CXX) $(SFML_INCLUDE) -I src -c $< -o $@

src/Objects/obj/Circle.o: src/Objects/Circle.cpp src/Objects/obj/Object.o
	$(CXX) $(SFML_INCLUDE) -I src -c $< -o $@

src/Objects/obj/Rectangle.o: src/Objects/Rectangle.cpp src/Objects/obj/Object.o
	$(CXX) $(SFML_INCLUDE) -I src -c $< -o $@

src/Objects/obj/Object.o: src/Objects/Object.cpp
	$(CXX) $(SFML_INCLUDE) -I src -c $< -o $@