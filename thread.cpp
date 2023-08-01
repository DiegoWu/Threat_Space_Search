#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <set>
#include <functional>
#include <stdexcept>
#include <fstream>
#define mp make_pair
// std::cerr;
std::ostream &info = std::cout;
std::ostream &debug = *(new std::ofstream);
// std::ostream &debug = std::cout;
std::ostream &chck= *(new std::ofstream);
const int BOARD_SIZE = 15;
const char EMPTY = '-';
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';

int iteration = 0;

class Gomoku
{

  const int DIR_X[8] = {1, 0, -1, 0, 1, -1, -1, 1};
  const int DIR_Y[8] = {0, 1, 0, -1, 1, 1, -1, -1};
  const char *FIVE_01 = "-OOOO-";
  const char *FIVE_02 = "OOOO-";
  const char *FIVE_03 = "O-OOO";
  const char *FIVE_04 = "OO-OO";

  const char *FOUR_00 = "--OOO--";
  const char *FOUR_01 = "-OOO--";
  const char *FOUR_02 = "OOO--";

  const char *FOUR_03 = "-OO-O-";
  const char *FOUR_04 = "OO-O-";
  const char *FOUR_05 = "-OO-O";

  const char *FOUR_06 = "OO--O";
  const char *FOUR_07 = "O-O-O";

  const char *THREE_07 = "--O-O--";
  const char *THREE_07_1 = "---OO--";
  const char *THREE_06 = "--O-O-";
  const char *THREE_06_0 = "-O--O-";
  const char *THREE_06_1 = "---OO-";
  const char *THREE_06_2 = "--OO--";

  const char *FIVE_1 = "-XXXX-";
  const char *FIVE_2 = "XXXX-";
  const char *FIVE_3 = "X-XXX";
  const char *FIVE_4 = "XX-XX";

  const char *FOUR_0 = "--XXX--";
  const char *FOUR_1 = "-XXX--";
  const char *FOUR_2 = "XXX--";

  const char *FOUR_3 = "-XX-X-";
  const char *FOUR_4 = "XX-X-";
  const char *FOUR_5 = "-XX-X";

  const char *FOUR_6 = "XX--X";
  const char *FOUR_7 = "X-X-X";
  /* --XXX--  --XXX- -XX-X-  */

  const char *THREE_7 = "--X-X--";
  const char *THREE_7_1 = "---XX--";
  const char *THREE_6 = "--X-X-";
  const char *THREE_6_0 = "-X--X-";
  const char *THREE_6_1 = "---XX-";
  const char *THREE_6_2 = "--XX--";

public:

  std::map<int,bool> visit_map;

  struct board
  {
    int turn;
    char type;
  };

  struct TSSNode
  {
    std::pair<int, int> gainSquare;
    std::set<std::pair<int, int>> costSquares;
    int depth = 1;
    std::vector<std::vector<board>> board_;
    std::string typeOfThreats;
    int player = 0;

    bool operator<(const TSSNode &other) const
    {
      std::string temp = "", temp1 = "";
      for (auto &pair : costSquares)
      {
        temp += std::to_string(pair.first) + std::to_string(pair.second);
      }
      for (auto &pair : other.costSquares)
      {
        temp1 += std::to_string(pair.first) + std::to_string(pair.second);
      }
      return temp1 > temp ? true : false;
    }
    bool operator==(const TSSNode &other) const
    {
      int cnt = 0;
      for (auto &pair : costSquares)
      {
        if (other.costSquares.find(pair) != other.costSquares.end())
          cnt++;
      }

      return gainSquare == other.gainSquare && cnt != costSquares.size() && costSquares == other.costSquares ? true : false;
    }
  };

private:

  bool terminate(std::string s)
  {
    int cnt = 0, cnt1 = 0;
    for (auto &x : s)
    {
      if (x == 'X')
        cnt++;
      if (x == 'O')
        cnt1++;
    }
    return cnt >= 4 || cnt1 >= 4 ? true : false;
  }

  char re(int a)
  {
    if (!a)
      return 'X';
    return a == -1 ? '-' : 'O';
  }

  int de = 0;

  std::pair<std::vector<std::pair<int, int>>, bool> dfs(std::map<std::string, std::pair<TSSNode, bool>> visited, TSSNode startNode, std::map<std::string, TSSNode> m, int player, int depth, int path_value)
  {
    debug<< "visit map size: "<< visit_map.size()<<std::endl;
    int now_x = startNode.gainSquare.first + 1;
    int now_y = startNode.gainSquare.second + 1;
    int point_value = (depth << 9) + (player << 8) + (now_x << 4) + (now_y << 0);
    path_value ^= point_value;

    debug << "enter" << std::endl;
    // std::string store= strinify(startNode.gainSquare, startNode.costSquares);
    visited[strinify(startNode.gainSquare, startNode.costSquares)] = std::mp(startNode, true);
    std::vector<std::pair<int, int>> path;
    path.push_back(std::mp(startNode.gainSquare.first, startNode.gainSquare.second));
    debug << "depth " << depth << std::endl;
    debug << startNode.typeOfThreats << std::endl;

    if (terminate(startNode.typeOfThreats))
    {
      debug << "return" << std::endl;
      de++;
      visit_map[path_value] == 1;
      return std::mp(path, true);
    }
    else if (depth == 100)
    {
      debug << "failed" << std::endl;
      visit_map[path_value] == 2;
      return std::mp(path, false);
    }

    // check if visited
    if(visit_map[path_value] == 1){ 
      de++;
      return std::mp(path, true);
    }
    else if(visit_map[path_value] == 2){
      return std::mp(path, false);
    }

    bool ck = false;
    debug << startNode.gainSquare.first + 1 << " " << startNode.gainSquare.second + 1 << " " << depth << std::endl;
    startNode.board_[startNode.gainSquare.first][startNode.gainSquare.second].turn = player;
    startNode.board_[startNode.gainSquare.first][startNode.gainSquare.second].type = re(player);

    int cnt = 0;
    std::set<std::pair<int, int>> v = startNode.costSquares;
    for (auto &c : startNode.costSquares)
    {
      startNode.board_[c.first][c.second].turn = (player + 1) % 2;
      startNode.board_[c.first][c.second].type = re((player + 1) % 2);
      // debug<< c.first<<" "<<re(player+1)<<std::endl;
      std::map<std::string, TSSNode> tt;

      for (int i = 0; i < 15; i++)
      {
        for (int j = 0; j < 15; j++)
        {
          debug << startNode.board_[i][j].type << " ";
        }
        debug << std::endl;
      }

      tt = updateThreat(tt, startNode.gainSquare.first, startNode.gainSquare.second, startNode.board_, player);

      debug << "threats size: " << tt.size() << std::endl;

      bool che = 0;
      if (tt.size() == 0)
        dfs(visited, startNode, tt, player, depth + 1, path_value);

      for (auto &pair : tt)
      {
        if (startNode.board_[pair.second.gainSquare.first][pair.second.gainSquare.second].turn != -1)
          continue;
        if (pair.first == strinify(startNode.gainSquare, startNode.costSquares) || visited[strinify(pair.second.gainSquare, pair.second.costSquares)].second == 1)
          continue;
        if (pair.second.costSquares.size() == 1 && pair.second.costSquares.find(startNode.gainSquare) != pair.second.costSquares.end())
        {
          debug << "continue" << std::endl;
          continue;
        }
        bool ch = 0;
        for (auto &x : visited)
        {
          if (x.second.first.costSquares.size() == 1 && x.second.first.costSquares.find(pair.second.gainSquare) != x.second.first.costSquares.end())
          {
            ch = 1;
            break;
          }
        }
        if (ch)
        {
          debug << "visited" << std::endl;
          continue;
        }

        debug << "pass 0" << std::endl;
        debug << pair.second.gainSquare.first + 1 << " " << pair.second.gainSquare.second + 1 << " " << depth + 1 << std::endl;

        pair.second.board_ = startNode.board_;
        std::pair<std::vector<std::pair<int, int>>, bool> pp = dfs(visited, pair.second, tt, player, depth + 1, path_value);
        
        if (pp.second){
          che = 1;
          path= pp.first; 
        }
        pair.second.board_ = startNode.board_;
        debug << "end" << std::endl;
      }
      if (che)
        cnt++;
      startNode.board_[c.first][c.second].turn = -1;
      startNode.board_[c.first][c.second].type = re(-1);
    }

    if (cnt == v.size()){
      ck = true;
      path.push_back(std::mp(startNode.gainSquare.first, startNode.gainSquare.second)); 
    }
    visit_map[path_value] = ck ? 1 : 2;
    return std::mp(path, ck);
  }

public:

  std::map<std::string, TSSNode> threatMap;
  std::set<std::pair<int, int>> r;
  std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> p;

  Gomoku(){}

  std::string strinify(std::pair<int, int> a, std::set<std::pair<int, int>> b)
  {
    std::string temp = "";
    temp += std::to_string(a.first) + std::to_string(a.second);
    for (auto &pair : b)
    {
      temp += std::to_string(pair.first) + std::to_string(pair.second);
    }
    return temp;
  }

  std::map<std::string, TSSNode> checkThreat(std::vector<std::vector<board>> board_, std::map<std::string, TSSNode> threat, int x, int y, const char *goal, char *t, int dir, int player)
  {
    iteration++;
    //chck<<"goal: "<<goal<<std::endl;
    std::vector<std::pair<int, int>> store;
    if (t == nullptr || strlen(t) <=4)
      return threat;
    char *str;
  
    int size = strlen(goal);
    std::set<std::pair<int, int>> record;
    // std::cout<<t<<std::endl;
    int num =0; 
    while ((str = strstr(t, goal)) != nullptr)
    {
      if(!num)  x += (str - t) * DIR_X[dir], y += (str - t) * DIR_Y[dir];
      else x+= (str-t+1)*DIR_X[dir], y+= (str-t+1)*DIR_Y[dir];
      num++; 
      chck<<x+1<<" "<<y+1<<" "<<goal<<" "<<dir<<std::endl; 
      bool flag = false;
      for (int i = 0; i < size; i++)
      {
        if (board_[x + DIR_X[dir] * i][y + DIR_Y[dir] * i].type == EMPTY)
          record.insert(std::mp(x + DIR_X[dir] * i, y + DIR_Y[dir] * i));
      }

      if (goal == THREE_7 || goal == THREE_7_1 || goal == THREE_07 || goal == THREE_07_1)
      {
        TSSNode node;
        for (int i = 2; i < 5; i++)
        {
          if (board_[x + DIR_X[dir] * i][y + DIR_Y[dir] * i].type == EMPTY)
            node.gainSquare = std::mp(x + DIR_X[dir] * i, y + DIR_Y[dir] * i);
        }
        node.costSquares.insert(std::mp(x + DIR_X[dir] * 1, y + DIR_Y[dir] * 1));
        node.costSquares.insert(std::mp(x + DIR_X[dir] * 5, y + DIR_Y[dir] * 5));
        node.typeOfThreats = goal;
        node.player = player;
        std::string tt = strinify(node.gainSquare, node.costSquares);
        threat[tt] = node;
      }

      else if (goal== THREE_6_0 || goal== THREE_06_0){

        for (int i=0; i<2; i++){
          TSSNode node;
          node.gainSquare = std::mp(x + DIR_X[dir] * (i+2), y + DIR_Y[dir] * (i+2));
          for (int j=0; j<6; j++){
            if(x+DIR_X[dir] * j == node.gainSquare.first && y+DIR_Y[dir] * j == node.gainSquare.second|| board_[x + DIR_X[dir] * j][y + DIR_Y[dir] * j].type != EMPTY)
              continue;
            node.costSquares.insert(std::mp(x + DIR_X[dir] * j, y + DIR_Y[dir] * j));
          }
          node.typeOfThreats = goal;
          node.player = player;
          std::string tt = strinify(node.gainSquare, node.costSquares);
          threat[tt] = node;
        }

      }
      else if (goal == THREE_6 || goal == THREE_6_1 || goal == THREE_06 || goal == THREE_06_1 || goal == THREE_06_2 || goal == THREE_6_2)
      {
        for (int i = 1; i < 5; i++)
        {
          TSSNode node;
          if (board_[x + DIR_X[dir] * i][y + DIR_Y[dir] * i].type != EMPTY)
            continue;
          // check duplicate cases e.g. --X-X- with --X-X--, ---XX- with ---XX--
          if (isValid(x + DIR_X[dir] * (i + 1), y + DIR_Y[dir] * (i + 1)) && isValid(x + DIR_X[dir] * (i - 1), y + DIR_Y[dir] * (i - 1)) && board_[x + DIR_X[dir] * (i + 1)][y + DIR_Y[dir] * (i + 1)].turn == player && board_[x + DIR_X[dir] * (i - 1)][y + DIR_Y[dir] * (i - 1)].turn == player && isValid(x + DIR_X[dir] * 6, y + DIR_Y[dir] * 6) && board_[x + DIR_X[dir] * 6][y + DIR_Y[dir] * 6].type == EMPTY)
            continue;
          if (isValid(x + DIR_X[dir] * (i + 1), y + DIR_Y[dir] * (i + 1)) && isValid(x + DIR_X[dir] * (i + 2), y + DIR_Y[dir] * (i + 2)) && board_[x + DIR_X[dir] * (i + 1)][y + DIR_Y[dir] * (i + 1)].turn == player && board_[x + DIR_X[dir] * (i + 2)][y + DIR_Y[dir] * (i + 2)].turn == player && isValid(x + DIR_X[dir] * 6, y + DIR_Y[dir] * 6) && board_[x + DIR_X[dir] * 6][y + DIR_Y[dir] * 6].type == EMPTY)
            continue;
          node.gainSquare = std::mp(x + DIR_X[dir] * i, y + DIR_Y[dir] * i);
          node.player = player;
          //r.insert(node.gainSquare);

          for (auto &pair : record)
            if (pair != node.gainSquare)
              node.costSquares.insert(pair);
          node.typeOfThreats = goal;
          std::string tt = strinify(node.gainSquare, node.costSquares);
          threat[tt] = node;
        }
      }
      else if (goal == FOUR_0 || goal == FOUR_00)
      {
        TSSNode node;
        node.gainSquare = std::mp(x + DIR_X[dir] * 1, y + DIR_Y[dir] * 1);
        node.costSquares.insert(std::mp(x, y));
        node.costSquares.insert(std::mp(x + DIR_X[dir] * 5, y + DIR_Y[dir] * 5));
        p[std::mp(node.gainSquare.first, node.gainSquare.second)].push_back(std::mp(x, y));
        p[std::mp(node.gainSquare.first, node.gainSquare.second)].push_back(std::mp(x + DIR_X[dir] * 5, y + DIR_Y[dir] * 5));
        node.typeOfThreats = goal;
        node.player = player;
        std::string tt = strinify(node.gainSquare, node.costSquares);
        threat[tt] = node;
        node.costSquares.clear();

        node.gainSquare = std::mp(x + DIR_X[dir] * 5, y + DIR_Y[dir] * 5);
        node.costSquares.insert(std::mp(x + DIR_X[dir], y + DIR_Y[dir]));
        node.costSquares.insert(std::mp(x + DIR_X[dir] * 6, y + DIR_Y[dir] * 6));
        p[std::mp(node.gainSquare.first, node.gainSquare.second)].push_back(std::mp(x + DIR_X[dir], y + DIR_Y[dir]));
        p[std::mp(node.gainSquare.first, node.gainSquare.second)].push_back(std::mp(x + DIR_X[dir] * 5, y + DIR_Y[dir] * 5));
        node.typeOfThreats = goal;
        node.player = player;
        tt = strinify(node.gainSquare, node.costSquares);
        threat[tt] = node;
      }
      else if (goal == FOUR_1 || goal == FOUR_01)
      {
        TSSNode node;
        node.gainSquare = std::mp(x + DIR_X[dir] * 4, y + DIR_Y[dir] * 4);
        node.costSquares.insert(std::mp(x, y));
        node.costSquares.insert(std::mp(x + DIR_X[dir] * 5, y + DIR_Y[dir] * 5));
        p[std::mp(node.gainSquare.first, node.gainSquare.second)].push_back(std::mp(x, y));
        p[std::mp(node.gainSquare.first, node.gainSquare.second)].push_back(std::mp(x + DIR_X[dir] * 5, y + DIR_Y[dir] * 5));
        std::string tt = strinify(node.gainSquare, node.costSquares);
        node.typeOfThreats = goal;
        node.player = player;
        threat[tt] = node;
      }
      else if (goal == FOUR_2 || goal == FOUR_02)
      {
        if (!isValid(x - DIR_X[dir], y - DIR_Y[dir]) || board_[x - DIR_X[dir]][y - DIR_Y[dir]].type != EMPTY)
          flag = 1;
      }

      else if (goal == FOUR_03 || goal == FOUR_3)
      {
        TSSNode node;
        node.gainSquare = std::mp(x + DIR_X[dir] * 3, y + DIR_Y[dir] * 3);
        node.costSquares.insert(std::mp(x, y));
        node.costSquares.insert(std::mp(x + DIR_X[dir] * 5, y + DIR_Y[dir] * 5));
        node.typeOfThreats = goal;
        node.player = player;
        threat[strinify(node.gainSquare, node.costSquares)] = node;
        p[std::mp(node.gainSquare.first, node.gainSquare.second)].push_back(std::mp(x, y));
        p[std::mp(node.gainSquare.first, node.gainSquare.second)].push_back(std::mp(x + DIR_X[dir] * 5, y + DIR_Y[dir] * 5));
      }
      else if (goal == FOUR_4 || goal == FOUR_04)
      {
        if (!isValid(x - DIR_X[dir], y - DIR_Y[dir]) || board_[x - DIR_X[dir]][y - DIR_Y[dir]].type != EMPTY)
          flag = 1;
      }
      else if (goal == FOUR_5 || goal == FOUR_05)
      {
        if (!isValid(x + DIR_X[dir] * 5, y + DIR_X[dir] * 5) || board_[x + DIR_X[dir] * 5][y + DIR_Y[dir] * 5].type != EMPTY)
          flag = 1;
      }
      /*
      else if(goal== FOUR_7|| goal== FOUR_07){
         for (int i=0; i<2; i++){
          TSSNode node;
          if(!isValid(x + DIR_X[dir] * (1+i*2), y + DIR_Y[dir] * (1+i*2)) || board_[x + DIR_X[dir] * (1+i*2)][y + DIR_Y[dir] * (1+i*2)].type != EMPTY)
            break; 
          if(!isValid(x + DIR_X[dir] * (3-i*2), y + DIR_Y[dir] * (3-i*2)) || board_[x + DIR_X[dir] * (3-i*2)][y + DIR_Y[dir] * (3-i*2)].type != EMPTY)
            break;
          node.gainSquare = std::mp(x + DIR_X[dir] * (1+i*2), y + DIR_Y[dir] * (1+i*2));
          node.costSquares.insert(std::mp(x + DIR_X[dir] * (3-i*2), y + DIR_Y[dir] * (3-i*2)));
          node.typeOfThreats = goal;
          node.player = player;
          std::string tt = strinify(node.gainSquare, node.costSquares);
          threat[tt] = node;
        }
      }
      */
      else if (goal == FOUR_6 || goal == FOUR_06  || terminate(goal))
        flag = 1;

      if (flag)
      {
        for (int i = 0; i < size; i++)
        {
          if (!isValid(x + DIR_X[dir] * i, y + DIR_Y[dir] * i) || board_[x + DIR_X[dir] * i][y + DIR_Y[dir] * i].type != EMPTY)
            continue;
          TSSNode node;
          node.gainSquare = std::mp(x + DIR_X[dir] * i, y + DIR_Y[dir] * i);
          node.player = player;
          //r.insert(std::mp(x + DIR_X[dir] * i, y + DIR_Y[dir] * i));

          node.typeOfThreats = goal;

          for (auto &pair : record)
          {
            if (pair == node.gainSquare)
              continue;
            node.costSquares.insert(pair);

            //p[std::mp(node.gainSquare.first, node.gainSquare.second)].push_back(pair);
          }

          std::string tt = strinify(node.gainSquare, node.costSquares);
          
          threat[tt] = node;
        }
      }

      t = str + 1;
      record.clear();
    }
    return threat;
  }

  bool isValid(int x, int y)
  {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE ? true : false;
  }

  std::map<std::string, TSSNode> updateThreat(std::map<std::string, TSSNode> m, int x, int y, std::vector<std::vector<board>> b, int player)
  {

    for (int i = 0; i < 8; i++)
    {
      std::vector<char> threat;
      char *temp;
      int need = -100;
      for (int j = 0; j < BOARD_SIZE; j++)
      {
        if (!isValid(x - DIR_X[i] * j, y - DIR_Y[i] * j))
          break;
        need = j;
      }
      if (need == -100)
        continue;
      for (int j = 0; j < BOARD_SIZE; j++)
      {
        if (!isValid(x - DIR_X[i] * need + DIR_X[i] * j, y - DIR_Y[i] * need + DIR_Y[i] * j))
          break;
        threat.push_back((char)b[x - DIR_X[i] * need + DIR_X[i] * j][y - DIR_Y[i] * need + DIR_Y[i] * j].type);
      }
      // debug<<x<<" "<<y<<std::endl;
      x = x - DIR_X[i] * need, y = y - DIR_Y[i] * need;
      threat.push_back('\0');
      temp = threat.data();
      // debug<<temp<<std::endl;
      if (player)
      {
        m = checkThreat(b, m, x, y, FIVE_01, temp, i, player);
        m = checkThreat(b, m, x, y, FIVE_02, temp, i, player);
        m = checkThreat(b, m, x, y, FIVE_03, temp, i, player);
        m = checkThreat(b, m, x, y, FIVE_04, temp, i, player);

        m = checkThreat(b, m, x, y, FOUR_00, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_01, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_02, temp, i, player);

        m = checkThreat(b, m, x, y, FOUR_03, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_04, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_05, temp, i, player);

        m = checkThreat(b, m, x, y, FOUR_06, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_07, temp, i, player);

        m = checkThreat(b, m, x, y, THREE_07_1, temp, i, player);
        m = checkThreat(b, m, x, y, THREE_07, temp, i, player);
        m = checkThreat(b, m, x, y, THREE_06, temp, i, player);
        m= checkThreat(b, m, x, y, THREE_06_0, temp, i, player);
        m = checkThreat(b, m, x, y, THREE_06_1, temp, i, player);
        m = checkThreat(b, m, x, y, THREE_06_2, temp, i, player);
      }
      else
      {
        m = checkThreat(b, m, x, y, FIVE_1, temp, i, player);
        m = checkThreat(b, m, x, y, FIVE_2, temp, i, player);
        m = checkThreat(b, m, x, y, FIVE_3, temp, i, player);
        m = checkThreat(b, m, x, y, FIVE_4, temp, i, player);

        m = checkThreat(b, m, x, y, FOUR_0, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_1, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_2, temp, i, player);

        m = checkThreat(b, m, x, y, FOUR_3, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_4, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_5, temp, i, player);

        m = checkThreat(b, m, x, y, FOUR_6, temp, i, player);
        m = checkThreat(b, m, x, y, FOUR_7, temp, i, player);

        m = checkThreat(b, m, x, y, THREE_7_1, temp, i, player);
        m = checkThreat(b, m, x, y, THREE_7, temp, i, player);
        m = checkThreat(b, m, x, y, THREE_6, temp, i, player);
        m= checkThreat(b, m, x, y, THREE_6_0, temp, i, player); 
        m = checkThreat(b, m, x, y, THREE_6_1, temp, i, player);
        m = checkThreat(b, m, x, y, THREE_6_2, temp, i, player);
      }
      x = x + DIR_X[i] * need, y = y + DIR_Y[i] * need;
    }
    return m;
  }

  std::map<std::string, TSSNode> findThreat(std::vector<std::vector<board>> b, int player)
  {
    for (int x = 0; x < BOARD_SIZE; x++)
    {

      for (int y = 0; y < BOARD_SIZE; y++)
      {

        threatMap = updateThreat(threatMap, x, y, b, player);
        // debug<<threatMap.size()<<" ";
      }
      // debug<<std::endl;
    }
    debug << "threats size: " << threatMap.size() << std::endl;
    int cnt = 0;
    std::map<std::string, std::pair<TSSNode, bool>> t;
    std::map<std::string, TSSNode> temp = threatMap;
    for (auto it = threatMap.begin(); it != threatMap.end(); ++it)
    {
      it->second.board_ = b;

      //if (it->second.gainSquare.first + 1 == 1 && it->second.gainSquare.second + 1 == 5)
      //{
        
      info<<"start: "<<it->second.gainSquare.first + 1<<" "<<it->second.gainSquare.second + 1<<std::endl;
     
      std::pair<std::vector<std::pair<int, int>>, bool> ck = dfs(t, it->second, temp, player, 1, 0);
      temp = threatMap;
      if (ck.second){
        cnt++;
        for (auto &ck : ck.first)
          info<<"<-"<<ck.first+1<<" "<< ck.second+1;  
        info<<std::endl;  
      }
      
      
      //}
    }
    info << "cnt: " << cnt << std::endl;
    debug << "possible winning tracks: " << de << std::endl;

    return threatMap;
  }

public:

  std::vector<std::vector<board>> bb = std::vector<std::vector<board>>(BOARD_SIZE, std::vector<board>(BOARD_SIZE, {-1, '-'}));
  int current_player_;
};

int main()
{
  Gomoku game;
  std::cout << "hi" << std::endl;
  char arr[15][15] = {
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', 'O', 'X', '-', 'X', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', 'O', 'X', 'X', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', 'X', 'O', 'X', '-', '-', '-', 'O', '-', '-', '-'},
      {'-', '-', '-', '-', 'O', '-', '-', 'O', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'}};
  char arr2[15][15] = {
      {'X', 'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'X', 'X', 'X'},
      {'X', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-', 'X'},
      {'X', '-', '-', '-', '-', '-', 'X', '-', 'O', '-', '-', '-', '-', '-', 'X'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', 'X', 'O', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', 'O', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'X', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'X'},
      {'X', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'X'},
      {'X', 'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'X', 'X', 'X'}};
  char arr1[15][15] = {
      {'X', 'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', 'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', 'X', 'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'}};
  char arr3[15][15] = {
      {'X', 'X', 'X', 'X', 'O', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', 'O', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', 'O', '-', 'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', 'O', '-', 'X', '-', '-', '-', '-', '-', 'O', '-', '-', '-', '-', '-'},
      {'-', '-', '-', 'X', '-', '-', '-', '-', 'X', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', 'X', 'O', 'X', 'X', 'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', 'O', '-', 'X', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
      {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'}}; 
  for (int i = 0; i < 15; i++)
    std::cout << " " << (i + 1) % 10;
  std::cout << std::endl;
  for (int i = 0; i < 15; i++)
  {
    std::cout << (i + 1) % 10;
    for (int j = 0; j < 15; j++)
    {
      game.bb[i][j].type = arr1[i][j];
      std::cout << game.bb[i][j].type << " ";
      if (arr1[i][j] == 'O')
        game.bb[i][j].turn = 1;
      else if (arr1[i][j] == 'X')
        game.bb[i][j].turn = 0;
      else
        game.bb[i][j].turn = -1;
    }
    std::cout << std::endl;
  }

  std::map<std::string, Gomoku::TSSNode> m = game.findThreat(game.bb, 0);
  std::set<std::pair<int, int>> r = game.r;
  std::cout << m.size() << std::endl;

  for (auto i = m.begin(); i != m.end(); i++)
  {
    arr1[i->second.gainSquare.first][i->second.gainSquare.second] = 'T';
    std::cout<<i->second.gainSquare.first + 1<<" "<<i->second.gainSquare.second + 1<<" "<<i->second.typeOfThreats<<std::endl;
    for (auto &pair : i->second.costSquares)
    {
      // std::cout << pair.first + 1 << " " << pair.second + 1 << std::endl;
      if (arr1[pair.first][pair.second] == 'T'){
        arr1[pair.first][pair.second] = 'T';
      }
      else
        arr1[pair.first][pair.second] = 'C';
    }
    std::cout << std::endl;
  }
  std::cout << "----------------------------------------------" << std::endl;
for (int i = 0; i < 15; i++)
    std::cout << " " << (i + 1) % 10;
std::cout<<std::endl;
  for (int i = 0; i < 15; i++)
  {
    std::cout << (i + 1) % 10;
    for (int j = 0; j < 15; j++)
    {
      std::cout << arr1[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << iteration << '\n';
  return 0;
}
