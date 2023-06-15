#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Cau truc cua mot dinh
struct Vertex {
    int index;  //chi so cua dinh
    int color;  //mau cua dinh
    int numNeighbors;   //so luong dinh ke
    struct Vertex** neighbors;  //danh sach cac dinh ke
    int saturation; //do bao hoa 
};

//Ham tao dinh (Nhan tham so dau vao la Ten dinh va So dinh ke)
struct Vertex* createVertex(int index, int numVertices) {
    struct Vertex* vertex = (struct Vertex*)malloc(sizeof(struct Vertex)); //Cap phat bo nho cho dinh
    vertex->index = index; //Gan ten dinh
    vertex->color = -1; //Khoi tao gia tri khi dinh chua to mau la -1
    vertex->numNeighbors = 0;   //So luong dinh ke = -1
    vertex->neighbors = (struct Vertex**)malloc(numVertices * sizeof(struct Vertex*));  //Cap phat bo nho cho mot mang cac con tro toi cac dinh ke
    vertex->saturation = 0; //Do bao hoa = 0
    return vertex;
}
//Do bao hoa cua mot dinh duoc tinh bang so mau khac nhau duoc su dung boi cac dinh ke

void updateSaturation(struct Vertex** vertices, int numVertices) {
    for (int i = 0; i < numVertices; ++i) {
        struct Vertex* vertex = vertices[i];
        //Kiem tra xem dinh to mau chua, chi xu li cac dinh chua to mau
        if (vertex->color == -1) {

            //Tao mot mang boolean danh dau cac mau da duoc su dung boi cac dinh ke 
            bool* usedColors = (bool*)calloc(numVertices, sizeof(bool));

            //Duyet qua tat ca dinh ke cua dinh hien tai
            for (int j = 0; j < vertex->numNeighbors; ++j) {
                struct Vertex* neighbor = vertex->neighbors[j];
                //Kiem tra xem dinh ke da to mau chua
                if (neighbor->color != -1) {
                    //Danh dau mau da duoc su dung
                    usedColors[neighbor->color] = true;
                }
            }
            vertex->saturation = 0;
            //Dem so luong mau da duoc su dung (do bao hoa) bang cach dem so phan tu "true" trong mang usedColors
            for (int j = 0; j < numVertices; ++j) {
                if (usedColors[j]) {
                    vertex->saturation++;
                }
            }
            free(usedColors);
        }
    }
}

//Ham tim dinh chua duoc to mau co do bao hoa lon nhat
int findNextVertex(struct Vertex** vertices, int numVertices) {
    int maxSaturation = -1; //Khoi tao bien luu tru do bao hoa cao nhat = -1 trong cac dinh chua to mau
    int nextVertex = -1; //Bien luu tru chi so cua dinh co do bao hoa cao nhat
    for (int i = 0; i < numVertices; ++i) {
        struct Vertex* vertex = vertices[i];
        if (vertex->color == -1 && vertex->saturation > maxSaturation) {
            maxSaturation = vertex->saturation;
            nextVertex = vertex->index;
        }
    }
    return nextVertex;
}

//Ham to mau chinh
void graphColoring(struct Vertex** vertices, int numVertices) {
    int numColors = 0; //Khoi tao bien numColors = 0. Day la so mau duoc su dung de to mau cac dinh
    while (true) {
        int nextVertex = findNextVertex(vertices, numVertices); //Goi ham findNextVertex tim dinh tiep theo can to mau
        if (nextVertex == -1) { //Neu khong con dinh nao (nextVertex == -1)
            break;
        }
        struct Vertex* vertex = vertices[nextVertex - 1]; //Lay dinh tiep theo tu mang vertices dua tren chi so nextVertex 
        if (vertex->color == -1) {
            //Tao mot mang usedColors de theo doi cac mau da su dung boi cac dinh ke cua vertex
            bool* usedColors = (bool*)calloc(numVertices, sizeof(bool));
            //Duyet qua tat ca dinh ke de xem chung da to mau hay chua
            for (int i = 0; i < vertex->numNeighbors; ++i) {
                struct Vertex* neighbor = vertex->neighbors[i];
                //Neu da duoc to mau, danh dau trong mang usedColor = true
                if (neighbor->color != -1) {
                    usedColors[neighbor->color] = true;
                }
            }
            //Duyet va tim mau dau tien co gia tri = false
            int color;
            for (color = 0; color < numVertices; ++color) {
                if (!usedColors[color]) {
                    break;
                }
            }
            //Gan mau cho vertex = color, neu color > numColors, cap nhat them + 1
            vertex->color = color;
            if (color >= numColors) {
                numColors = color + 1;
            }
            free(usedColors);
        }
        //Goi ham updateSaturation de cap nhat do bao hoa
        updateSaturation(vertices, numVertices);
    }

    FILE* outputfile = fopen("dothitomau.txt", "w");
    fprintf(outputfile, "%d\n", numColors);
    for (int i = 0; i < numVertices; ++i) {
        struct Vertex* vertex = vertices[i];
        fprintf(outputfile, "%d\n", vertex->color + 1);
    }
    fclose(outputfile);
}

int main() {
    FILE* inputfile = fopen("dothi.txt", "r");
    if (inputfile == NULL) {
        printf("Can't open input file!\n");
        return 1;
    }
    //Doc so dinh va so canh
    int numVertices, numEdges;
    fscanf(inputfile, "%d %d", &numVertices, &numEdges);
    //Tao cac dinh va gan vao mang vertices
    struct Vertex** vertices = (struct Vertex**)malloc(numVertices * sizeof(struct Vertex*));
    for (int i = 0; i < numVertices; ++i) {
        vertices[i] = createVertex(i + 1, numVertices);
    }
    //Doc tu tep dau vao va xac dinh cac dinh ke cua tung dinh
    for (int i = 0; i < numEdges; ++i) {
        int v1, v2;
        fscanf(inputfile, "%d %d", &v1, &v2);
        vertices[v1 - 1]->neighbors[vertices[v1 - 1]->numNeighbors++] = vertices[v2 - 1];
        vertices[v2 - 1]->neighbors[vertices[v2 - 1]->numNeighbors++] = vertices[v1 - 1];
    }

    fclose(inputfile);
    //Goi ham de thuc hien to mau
    graphColoring(vertices, numVertices);
    //Giai phong bo nho
    for (int i = 0; i < numVertices; ++i) {
        free(vertices[i]->neighbors);
        free(vertices[i]);
    }
    free(vertices);

    return 0;
}
