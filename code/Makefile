
all: gen enumerate_node DC
	echo "finish"

enumerate_node: enumerate_node.cpp
	g++ -fopenmp enumerate_node.cpp -o enumerate_node

DC: DC_triangle_count.cpp
	g++ -fopenmp DC_triangle_count.cpp -o DC

gen: gen.cpp
	g++ gen.cpp -o gen

