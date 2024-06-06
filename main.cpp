#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>

#include "API.h"

std::pair<int, int> west_wall[1000];
std::pair<int, int> east_wall[1000];
std::pair<int, int> north_wall[1000];
std::pair<int, int> south_wall[1000];
int w = 0, e = 0, n = 0, s = 0;
std::string direct_mouse[5];
std::string wall_l, wall_r, wall_f;

void log(const std::string& text) {
    std::cerr << text << std::endl;
}
void mark_wall(int mx, int my, std::string &direct){
    if(direct == "w"){
        west_wall[w].first = mx;
        west_wall[w].second = my;
        w++;
    }
    else if(direct == "e"){
        east_wall[e].first = mx;
        east_wall[e].second = my;
        e++;
    }
    else if(direct == "n"){
        north_wall[n].first = mx;
        north_wall[n].second = my;
        n++;
    }
    else if(direct == "s"){
        south_wall[s].first = mx;
        south_wall[s].second = my;
        s++;
    }
}


bool check_wall(int mx, int my, std::string direct){
    int k = 0;
    if(direct == "w"){
        for(int i = 0; i < w; i++)
            if(west_wall[i].first == mx && west_wall[i].second == my)
                k++;
    }
    else if(direct == "e"){
        for(int i = 0; i < e; i++)
            if(east_wall[i].first == mx && east_wall[i].second == my){
                k++;
            }
                
    }
    else if(direct == "n"){
        for(int i = 0; i < n; i++)
            if(north_wall[i].first == mx && north_wall[i].second == my)
                k++;
    }
    else if(direct == "s"){
        for(int i = 0; i < s; i++)
            if(south_wall[i].first == mx && south_wall[i].second == my)
                k++;
    }
    if(k == 0)
        return false;
    else
        return true;
}
void mouse_way(int d){
    
    direct_mouse[1] = "n";
    direct_mouse[2] = "e";
    direct_mouse[3] = "s";
    direct_mouse[4] = "w";
    
    if(d == 1)
    {
        wall_f = "n";
        wall_l = "w";
        wall_r = "e";
    }
    else if(d == 2){
        wall_f = "e";
        wall_l = "n";
        wall_r = "s";
    }
    else if(d == 3){
        wall_f = "s";
        wall_l = "e";
        wall_r = "w";
    }
    else if(d == 4){
        wall_f = "w";
        wall_l = "s";
        wall_r = "n";
    }
}



void bfs(std::vector<std::vector<int>>& maze, int mx, int my){
    std::queue<std::pair<int, int>> q;
    //base case
    
    if(!check_wall(mx - 1, my, "s")){
        maze[mx - 1][my] = 1;q.push({mx - 1, my});
    }
    if(!check_wall(mx - 1, my + 1, "s")){
        maze[mx - 1][my + 1] = 1;q.push({mx - 1, my + 1});
    }
    if(!check_wall(mx, my + 2, "w")){
        maze[mx][my + 2] = 1;q.push({mx, my + 2});
    }
    if(!check_wall(mx + 1, my + 2, "w")){
        maze[mx + 1][my + 2] = 1;q.push({mx + 1, my + 2});
    }
    if(!check_wall(mx + 2, my + 1, "n")){
        maze[mx + 2][my + 1] = 1;q.push({mx + 2, my + 1});
    }
    if(!check_wall(mx + 2, my, "n")){
        maze[mx + 2][my] = 1;q.push({mx + 2, my});
    }
    if(!check_wall(mx + 1, my - 1, "e")){
        maze[mx + 1][my - 1] = 1;q.push({mx + 1, my - 1});
    }
    if(!check_wall(mx, my - 1, "e")){
        maze[mx][my - 1] = 1; q.push({mx, my - 1});
    }
    while(!q.empty()){
        int nx, ny;
        nx = q.front().first;
        ny = q.front().second;
        q.pop();
        if(!check_wall(nx + 1, ny, "n") && nx + 1 < 17 && maze[nx + 1][ny] == 0){ // down
            q.push({nx + 1, ny});
            maze[nx + 1][ny] = maze[nx][ny] + 1;
        }
        if(!check_wall(nx, ny - 1, "e") && ny - 1 >= 1 && maze[nx][ny - 1] == 0){ // left
            q.push({nx, ny - 1});
            maze[nx][ny - 1] = maze[nx][ny] + 1;
        }
        if(nx - 1 >= 1 && maze[nx - 1][ny] == 0 && !check_wall(nx - 1, ny, "s")){ // up
            q.push({nx - 1, ny});
            maze[nx - 1][ny] = maze[nx][ny] + 1;
        }
        if(ny + 1 < 17 && maze[nx][ny + 1] == 0 && !check_wall(nx, ny + 1, "w")){ // right
            q.push({nx, ny + 1});
            maze[nx][ny + 1] = maze[nx][ny] + 1;
        }
    }
    maze[mx][my] = maze[mx + 1][my] = maze[mx + 1][my + 1] = maze[mx][my + 1] = 0;
}

int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "Start");
    int d = 1;
    int mx = 16, my = 1;
    std::vector<std::vector<int>> maze(18,std::vector<int>(18, 0));
    
    while (true) {
        char str[1000];
        sprintf(str, "%d", mx);
        log(str);
        if(d < 1)
            d = 3;
        if(d > 4)
            d = 1;
        mouse_way(d);
        if(API::wallFront()) mark_wall(mx, my, wall_f);
        if(API::wallLeft()) mark_wall(mx, my, wall_l);
        if(API::wallRight()) mark_wall(mx, my, wall_r);
        bfs(maze, 8, 8);
        
        if(direct_mouse[d] == "n"){
            if(maze[mx][my + 1] <= maze[mx][my] && !check_wall(mx, my, "e")  && my + 1 < 17){
                my += 1;
                API::turnRight();
                API::moveForward();
                d++;

            }
            else if(maze[mx][my - 1] <= maze[mx][my] && !check_wall(mx, my, "w")  && my - 1 >= 1){
                my -= 1;
                API::turnLeft();
                API::moveForward();
                d--;
            }
            else if(maze[mx - 1][my] <= maze[mx][my] && !check_wall(mx, my, "n")  && mx - 1 >= 1){
                mx -= 1;
                API:: moveForward();
            }
            else {
                API::turnLeft();
                API::turnLeft();
                d -= 2;
            }
        }
        else if(direct_mouse[d] == "e"){
            if(maze[mx + 1][my] <= maze[mx][my] && !check_wall(mx, my, "s")  && mx + 1 < 17){
                mx += 1;
                API::turnRight();
                API::moveForward();
                d++;
            }
            else if(maze[mx - 1][my] <= maze[mx][my] && !check_wall(mx, my, "n")  && mx - 1 >= 1){
                mx -= 1;
                API::turnLeft();
                API::moveForward();
                d--;
            }
            else if(maze[mx][my + 1] <= maze[mx][my] && !check_wall(mx, my, "e")  && my + 1 < 17){
                my += 1;
                API::moveForward();
            }
            else{
                API::turnLeft();
                API::turnLeft();
                d -= 2;
            }
        }
        else if(direct_mouse[d] == "s"){
            if(maze[mx][my - 1] <= maze[mx][my] && !check_wall(mx, my, "w")  && my - 1 >= 1){
                my -= 1;
                API::turnRight();
                API::moveForward();
                d++;
            }
            else if(maze[mx][my + 1] <= maze[mx][my] && !check_wall(mx, my, "e")  && my + 1 < 17){
                my += 1;
                API::turnLeft();
                API::moveForward();
                d--;
            }
            else if(maze[mx + 1][my] <= maze[mx][my] && !check_wall(mx, my, "s")  && mx + 1 < 17){
                mx += 1;
                API::moveForward();
            }
            else{
                API::turnLeft();
                API::turnLeft();
                d -= 2;
            }
       
        }
        else if(direct_mouse[d] == "w"){
            if(maze[mx - 1][my] <= maze[mx][my] && !check_wall(mx, my, "n")  && mx - 1 >= 1){
                mx -= 1;
                API::turnRight();
                API::moveForward();
                d++;
            }
            else if(maze[mx + 1][my] <= maze[mx][my] && !check_wall(mx, my, "s")  && mx + 1 < 17){
                mx += 1;
                API::turnLeft();
                API::moveForward();
                d--;
            }
            else if(maze[mx][my - 1] <= maze[mx][my] && !check_wall(mx, my, "w") && my - 1 >= 1){
                my -= 1;
                API::moveForward();
            }
            else{
                API::turnLeft();
                API::turnLeft();
                d -= 2;
            }
        }
       /*  for (const auto& row : maze) {
            std::string st = "";
            for (int cell : row) {
                sprintf(str, "%d", cell);
                st = st + str + " ";
            }
            log(st);
        } */
        bfs(maze, 8, 8);
        for(int i = 0; i < e; i++){
            std::string st; 
            sprintf(str, "%d", north_wall[i].first);
            st = st + str;
            sprintf(str, "%d", north_wall[i].second);
            st = st + " " + str;
            log(st);
        }
        log(".......");
        
    }
}
