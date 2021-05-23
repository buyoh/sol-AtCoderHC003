#include "header.hpp"

enum Direction { Up, Right, Down, Left };
using Commands = vector<Direction>;

struct Field {
  static constexpr int N = 30;
  int hz[N][N]; // (y,x) から右に移動するコスト
  int vt[N][N]; // (y,x) から下に移動するコスト
  void fill(int v) {
    std::fill(hz[0], hz[N], v);
    std::fill(vt[0], vt[N], v);
  }
  bool check(int y, int x, Direction direction) const {
    return !((y == 0 && direction == Up) || (x == 0 && direction == Left) ||
             (y == N - 1 && direction == Down) ||
             (x == N - 1 && direction == Right)) &&
           0 <= x && 0 <= y && x < N && y < N;
  }
  int &operator()(int y, int x, Direction direction) {
    switch (direction) {
    case Up:
      return vt[y - 1][x];
    case Right:
      return hz[y][x];
    case Down:
      return vt[y][x];
    case Left:
      return hz[y][x - 1];
    }
    /* NOTREACHED */
    return hz[0][0];
  }
  int operator()(int y, int x, Direction direction) const {
    switch (direction) {
    case Up:
      return vt[y - 1][x];
    case Right:
      return hz[y][x];
    case Down:
      return vt[y][x];
    case Left:
      return hz[y][x - 1];
    }
    /* NOTREACHED */
    return hz[0][0];
  }
};

//

string convertDirectionsToString(const Commands &commands) {
  string str;
  str.reserve(commands.size());
  for (auto e : commands)
    str.push_back("URDL"[e]);
  return str;
}

Commands generateSimpleRoute(P source, P destination) {
  Commands cmds;
  P p = source;
  while (p.x < destination.x) {
    cmds.push_back(Right);
    p.x += 1;
  }
  while (p.x > destination.x) {
    cmds.push_back(Left);
    p.x -= 1;
  }
  while (p.y < destination.y) {
    cmds.push_back(Down);
    p.y += 1;
  }
  while (p.y > destination.y) {
    cmds.push_back(Up);
    p.y -= 1;
  }
  cmds.shrink_to_fit();
  return cmds;
}

Commands generateDijkstra(const Field &field, P source, P destination) {
  F<int> dist(30, 30);
  dist.fill(numeric_limits<int>::max() / 2);
  priority_queue<pair<int, P>> que;
  que.emplace(0, destination);
  dist(destination) = 0;
  while (!que.empty()) {
    auto pp = que.top();
    que.pop();
    auto p = pp.second;
    if (-pp.first > dist(p))
      continue;
    // TODO: break;
    repeat(d, 4) {
      auto v = FourMoving[d];
      auto p2 = p + v;
      if (!field.check(p.y, p.x, static_cast<Direction>(d)))
        continue;
      auto d2 = dist(p) + field(p.y, p.x, static_cast<Direction>(d));
      if (d2 < dist(p2)) {
        dist(p2) = d2;
        que.emplace(-d2, p2);
      }
    }
  }
  Commands cmds;
  {
    P p = source;
    while (!(p == destination)) {
      repeat(d, 4) {
        auto v = FourMoving[d];
        auto p2 = p + v;
        if (!field.check(p.y, p.x, static_cast<Direction>(d)))
          continue;
        auto d2 = dist(p) - field(p.y, p.x, static_cast<Direction>(d));
        if (d2 == dist(p2)) {
          p = p2;
          cmds.push_back(static_cast<Direction>(d));
          break;
        }
      }
    }
  }
  return cmds;
}

//

P last_source;
Commands last_command;
Field field;

void solverInitialize() {
  //
}

string solverSolveQuery(int query_idx, P source, P destination) {
  last_source = source;
  if (query_idx == 0) {
    last_command = generateSimpleRoute(source, destination);
    return convertDirectionsToString(last_command);
  }

  last_command = generateDijkstra(field, source, destination);
  return convertDirectionsToString(last_command);
}

void solverFeedback(int query_idx, int result) {
  result *= 10; // もっと大きな係数を掛けたいがオーバーフローしそう
  if (query_idx == 0) {
    int k = result / int(last_command.size());
    field.fill(k);
    return;
  }
  {
    P p = last_source;
    int k = result / int(last_command.size());
    for (auto d : last_command) {
      auto &e = field(p.y, p.x, d);
      e = (e + k) / 2;
      p += FourMoving[d];
    }
  }
}

void solve() {
  //
  solverInitialize();
  repeat(i, 1000) {
    P s, t;
    cin >> s.y >> s.x >> t.y >> t.x;
    cout << solverSolveQuery(i, s, t) << endl;
    int feedback;
    cin >> feedback;
    solverFeedback(i, feedback);
  }
}

int main() {
  solve();
  return 0;
}
