CXX := g++
SFML_INCLUDE := -I libs/SFML/include
SFML_LIB := -L libs/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system

INCLUDE := -I include

EXECUTABLE := build/test/main.exe
OBJECTS := objects/Rectangle.o objects/Circle.o objects/Object.o objects/PhysicsMath.o 
OBJECTS += objects/Engine.o objects/Physics.o objects/Vector2.o
OBJECTS += objects/Command.o objects/ObjectFactory.o objects/AbstractBox.o objects/Quadtree.o

$(EXECUTABLE): src/main.cpp $(OBJECTS)
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src $^ -o $@ $(SFML_LIB)

objects/Engine.o: src/Engine/Engine.cpp include/Engine.hpp objects/Object.o objects/PhysicsMath.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Physics.o: src/Engine/Physics.cpp include/Physics.hpp objects/Object.o objects/PhysicsMath.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/PhysicsMath.o: src/Math/PhysicsMath.cpp include/PhysicsMath.hpp objects/Object.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Command.o: src/Engine/Command.cpp include/Command.hpp objects/ObjectFactory.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/ObjectFactory.o: src/Engine/ObjectFactory.cpp include/ObjectFactory.hpp objects/Object.o objects/Circle.o objects/Rectangle.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Quadtree.o: src/Math/Quadtree.cpp include/Quadtree.hpp objects/Object.o objects/Circle.o objects/Rectangle.o objects/AbstractBox.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Circle.o: src/Objects/Circle.cpp include/Circle.hpp objects/Object.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Rectangle.o: src/Objects/Rectangle.cpp include/Rectangle.hpp objects/Object.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Object.o: src/Objects/Object.cpp include/Object.hpp objects/Vector2.o objects/AbstractBox.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@
	
objects/AbstractBox.o: src/Math/AbstractBox.cpp include/AbstractBox.hpp objects/Vector2.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Vector2.o: src/Math/Vector2.cpp include/Vector2.hpp
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@
	
clean:
	rm -rf build/test/main.exe objects/*.o