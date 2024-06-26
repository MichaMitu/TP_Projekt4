#include "planar_quadrotor_visualizer.h"
#include <math.h>

PlanarQuadrotorVisualizer::PlanarQuadrotorVisualizer(PlanarQuadrotor *quadrotor_ptr): quadrotor_ptr(quadrotor_ptr) {}

/**
 * TODO: Improve visualizetion
 * 1. DONE Transform coordinates from quadrotor frame to image frame so the circle is in the middle of the screen
 * 2. DONE Use more shapes to represent quadrotor (e.x. try replicate http://underactuated.mit.edu/acrobot.html#section3 or do something prettier)
 * 3. DONE Animate proppelers (extra points)
 */
void PlanarQuadrotorVisualizer::render(std::shared_ptr<SDL_Renderer> &gRenderer, int scale) {
    Eigen::VectorXf state = quadrotor_ptr->GetState();
    float q_x, q_y, q_theta;
    Eigen::Vector2i windowSize;
    SDL_GetRendererOutputSize(gRenderer.get(), &windowSize[0], &windowSize[1]);

    /* x, y, theta coordinates */
    q_x = state[0] * scale + windowSize[0] / 2;
    q_y = state[1] * scale + windowSize[1] / 2;
    q_theta = state[2];

    Sint16 px = (Sint16)q_x;
    Sint16 py = (Sint16)q_y;

    Sint16 vx[4] = {px - 10, px - 10, px + 10, px + 10};
    Sint16 vy[4] = {py - 10, py + 10, py + 10, py - 10};

    float cos_theta = sqrt(1 - q_theta * q_theta);
    int size = 60;
    int heigth = 30;
    int prop_size = 15;

    size /= 2;
    int lx[2] = {-cos_theta * size + px, cos_theta * size + px};
    int ly[2] = {q_theta * size + py, -q_theta * size + py};
    prop_size *= sin(SDL_GetTicks() / 50) + 1;

    SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0x00, 0x00, 0xFF);
    //filledPolygonColor(gRenderer.get(), vx, vy, 4, 0xFF0000FF);
    lineColor(gRenderer.get(), lx[0], ly[0], lx[1], ly[1], 0xFF0000FF);
    lineColor(gRenderer.get(), lx[0], ly[0], lx[0] - q_theta * heigth, ly[0] - cos_theta * heigth, 0xFF0000FF);
    lineColor(gRenderer.get(), lx[1], ly[1], lx[1] - q_theta * heigth, ly[1] - cos_theta * heigth, 0xFF0000FF);
    for(int i = 0; i < 2; i++) 
    {
        int rx = lx[i] - q_theta * heigth;
        int ry = ly[i] - cos_theta * heigth;
        Sint16 vx[4] = {rx, rx - prop_size, rx - prop_size, rx};
        Sint16 vy[4] = {ry, ry - 5, ry + 5, ry};
        bezierColor(gRenderer.get(), vx, vy, 4, 3, 0xFF0000FF);
        Sint16 wx[4] = {rx, rx + prop_size, rx + prop_size, rx};
        Sint16 wy[4] = {ry, ry - 5, ry + 5, ry};
        bezierColor(gRenderer.get(), wx, wy, 4, 3, 0xFF0000FF);
    }
}