
all: gen base DC
	echo "finish"

base: enumerate_node.cpp
	g++ -fopenmp enumerate_node.cpp -o base 

DC: DC_triangle_count.cpp
	g++ -fopenmp DC_triangle_count.cpp -o DC

gen: gen.cpp
	g++ gen.cpp -o gen

