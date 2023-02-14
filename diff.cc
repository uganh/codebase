
#include <algorithm>
#include <map>
#include <string>
#include <vector>

std::vector<std::pair<size_t, size_t>> solution(const std::vector<std::map<ssize_t, size_t>> &Vs, size_t n, size_t m, ssize_t d) {
  ssize_t x = n;
  ssize_t y = m;

  std::vector<std::pair<size_t, size_t>> ans;

  ans.push_back({x, y});

  for ( ; d > 0; --d) {
    const std::map<ssize_t, size_t> &V = Vs[d - 1];

    ssize_t k = x - y;
  
    if (k == -d || (k != d && V.at(k - 1) < V.at(k + 1))) {
      x = V.at(k + 1);
      y = x - (k + 1);
    } else {
      x = V.at(k - 1);
      y = y - (k - 1);
    }

    ans.push_back({x, y});
  }

  std::reverse(ans.begin(), ans.end());

  return ans;
}

std::vector<std::pair<size_t, size_t>> diff(const std::string &a, const std::string &b) {
  size_t n = a.length();
  size_t m = b.length();

  std::map<ssize_t, size_t> V;
  std::vector<std::map<ssize_t, size_t>> Vs;

  V[1] = 0;

  for (ssize_t d = 0; /* d <= n + m */; ++d) {
    for (ssize_t k = -d; k <= d; k += 2) {
      size_t x;
      if (k == -d || (k != d && V[k - 1] < V[k + 1])) {
        x = V[k + 1];
      } else {
        x = V[k - 1] + 1;
      }
      size_t y = x - k;
    
      while (x < n && y < m && a[x] == b[y]) {
        ++x;
        ++y;
      }

      V[k] = x;

      if (x >= n && y >= m) {
        return solution(Vs, n, m, d);
      }
    }

    Vs.push_back(V);
  }
}

int main(void) {
  std::string a = "ABCABBA";
  std::string b = "CBABAC";

  std::vector<std::pair<size_t, size_t>> ans = diff(a, b);

  size_t x = 0;
  size_t y = 0;

  printf("%s\n", a.c_str());
  printf("%s\n", b.c_str());

  for (size_t i = 0; i < ans.size(); ++i) {
    size_t x_ = ans[i].first;
    size_t y_ = ans[i].second;

    if ((x_ - y_) > (x - y)) {
      // delete
      printf("\033[1;31m-%c\033[0m\n", a[x++]);
    } else if ((x_ - y_) < (x - y)) {
      // insert
      printf("\033[1;42m+%c\033[0m\n", b[y++]);
    }

    while (x < x_) {
      printf(" %c\n", a[x]);
      ++x;
      ++y;
    }
  }

  return 0;
}
