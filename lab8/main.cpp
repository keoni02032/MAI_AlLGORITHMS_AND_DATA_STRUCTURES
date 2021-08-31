#include <iostream>
#include <vector>

int main() {

	int n;
	std::cin >> n;
	std::vector<int> sequence(n);
	for (int i = 0; i < n; ++i) {
		std::cin >> sequence[i];
	}

	int count1 = 0, count2 = 0;
	for (int i = 0; i < n; ++i) {
		if (sequence[i] == '1') {
			++count1;
			continue;
		} else if (sequence[i] == '2') {
			++count2;
			continue;
		} else {
			continue;
		}
	}

	int result = 0;

	for (int i = 0; i < n; ++i) {
		if (i < count1) {
			if (sequence[i] == '3') {
				for (int j = n - 1; j >= count1; --j) {
					if (sequence[j] == '1') {
						std::swap(sequence[i], sequence[j]);
						result++;
						// break;
					}
				}
			} else if (sequence[i] == '2') {
				for (int j = count1; j < n; ++j) {
					if (sequence[j] == '1') {
						std::swap(sequence[i], sequence[j]);
						result++;
						// break;
					}
				}
			}
		} else if (i < count1 + count2) {
			if (sequence[i] == '3') {
				for (int j = count1 + count2; j < n; ++j) {
					if (sequence[j] == '2') {
						std::swap(sequence[i], sequence[j]);
						result++;
						// break;
					}
				}
			}
		}
	}

	std::cout << result << std::endl;

	return 0;
}