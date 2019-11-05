#ifdef _WIN32
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#endif

#include "render_world.h"
#include "driver.h"
#include "film.h"
#include <iostream>
#include <sstream>

//--------------------------------------------------------------------------------
int width = 640;
int height = 480;
//--------------------------------------------------------------------------------
Render_World world;
Driver driver(world);

//--------------------------------------------------------------------------------
void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (world.camera.film.colors)
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, (GLvoid *) world.camera.film.colors);
    glFlush();
}

//--------------------------------------------------------------------------------
void Handle_Idle() {
    if (!driver.Render_More())
        Display();
}

//--------------------------------------------------------------------------------
void Initialize_Opengl_And_Glut(int argc, char **argv) {
    glutInit(&argc, (char **) argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("Ray Tracer");
    glutDisplayFunc(Display);
    glutIdleFunc(Handle_Idle);
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//--------------------------------------------------------------------------------
void Usage(const std::string &exec) {
    std::cout << "Usage: " << exec << " <test-file>" << "\n" << std::endl;
    std::cout << "       " << exec << " -p <test-file>" << "\n" << std::endl;
    std::cout << "       " << exec << " -d <test-file> <solution-file> <mask-file>" << "\n" << std::endl;
    exit(1);
}

void Parse(Render_World &world, int &width, int &height, const char *test_file);

void Dump_png(Pixel *data, int width, int height, const char *filename);

void Read_png(Pixel *&data, int &width, int &height, const char *filename);

int main(int argc, char **argv) {
    bool dump_png = false, compute_diff = false;
    char *solution_filename = 0;
    char *mask_filename = 0;
    if (argc == 5) {
        if (argv[1][0] != '-' || argv[1][1] != 'd' || argv[1][2]) Usage(argv[0]);
        compute_diff = true;
        argv[1] = argv[2];
        argc = 2;
        solution_filename = argv[3];
        mask_filename = argv[4];
    }
    if (argc == 3) {
        if (argv[1][0] != '-' || argv[1][1] != 'p' || argv[1][2]) Usage(argv[0]);
        dump_png = true;
        argv[1] = argv[2];
        argc--;
    }
    if (argc != 2) Usage(argv[0]);
    Parse(world, width, height, argv[1]);

    if (compute_diff) {
        while (!driver.Render_More()) {}
        int width2 = 0, height2 = 0;
        Pixel *data_sol = 0;
        Pixel *data_mask = 0;
        Read_png(data_sol, width2, height2, solution_filename);
        assert(width == width2);
        assert(height == height2);
        Read_png(data_mask, width2, height2, mask_filename);
        assert(width == width2);
        assert(height == height2);

        double error = 0, total = 0;
        for (int i = 0; i < height * width; i++) {
            vec3 a = From_Pixel(world.camera.film.colors[i]);
            vec3 b = From_Pixel(data_sol[i]);
            vec3 m = From_Pixel(data_mask[i]);
            for (int c = 0; c < 3; c++) {
                double e = fabs(a[c] - b[c]);
                error += e * m[c];
                total += m[c];
                b[c] = e;
                m[c] = e * m[c];
            }
            data_sol[i] = Pixel_Color(b);
            data_mask[i] = Pixel_Color(m);
        }
        printf("error: %.2f\n", error / total * 100);
        Dump_png(world.camera.film.colors, width, height, "output.png");
        Dump_png(data_sol, width, height, "diff.png");
        Dump_png(data_mask, width, height, "weighted-diff.png");
    } else if (dump_png) {
        while (!driver.Render_More()) {}
        if (world.antialias) world.camera.film.FinishAA();
        Dump_png(world.camera.film.colors, width, height, "output.png");
    } else {
        Initialize_Opengl_And_Glut(argc, argv);
        glutMainLoop();
    }

    return 0;
}

// int main() {
//     // int argc = 3;
//     // char *argv[5] = {"", "-p", "scene.txt", "render-21.png", "mask-all.png"};
//     return _main(argc, argv);
// }

