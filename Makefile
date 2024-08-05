CXX := g++
SFML_INCLUDE := -I libs/SFML/include
SFML_LIB := -L libs/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system

INCLUDE := -I include

EXECUTABLE := build/test/main.exe
OBJECTS := objects/Rectangle.o objects/Circle.o objects/Object.o objects/PhysicsMath.o 
OBJECTS += objects/Engine.o objects/Physics.o
OBJECTS += objects/Command.o objects/ObjectFactory.o

$(EXECUTABLE): src/main.cpp $(OBJECTS)
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src $^ -o $@ $(SFML_LIB)

objects/Engine.o: src/Engine/Engine.cpp objects/Object.o objects/PhysicsMath.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Physics.o: src/Engine/Physics.cpp objects/Object.o objects/PhysicsMath.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/PhysicsMath.o: src/Math/PhysicsMath.cpp objects/Object.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Command.o: src/Engine/Command.cpp objects/ObjectFactory.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/ObjectFactory.o: src/Engine/ObjectFactory.cpp objects/Object.o objects/Circle.o objects/Rectangle.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Circle.o: src/Objects/Circle.cpp objects/Object.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Rectangle.o: src/Objects/Rectangle.cpp objects/Object.o
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@

objects/Object.o: src/Objects/Object.cpp
	$(CXX) $(SFML_INCLUDE) $(INCLUDE) -I src -c $< -o $@