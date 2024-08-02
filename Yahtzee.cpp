#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <string>
using namespace std;

class DiceGame {
private:
    vector<int> dice;  // ���݂̃_�C�X�̒l���i�[����
    vector<int> categoryScores;  // �e���̓��_���i�[����
    int totalScore;  // ���v���_���i�[����ϐ�
    int upperSectionScore;  // �u1�̖ځv����u6�̖ځv�i�㕔�̖��j�̍��v���_���i�[����ϐ�
    random_device engine;  // �����̐���

public:
    DiceGame() : dice(5), categoryScores(13, -1), totalScore(0), upperSectionScore(0) {}

    // min����max�܂ł̗����𐶐�����֐�
    int getRandomInt(int min, int max) {
        uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }

    // 5�̃_�C�X��U��֐�
    void rollDice() {
        for (int& die : dice) {
            die = getRandomInt(1, 6);
        }
    }

    // ����̃_�C�X��U�蒼���֐�
    void rerollDice(int index) {
        dice[index] = getRandomInt(1, 6);
        cout << "�_�C�X��U�蒼���܂���: " << dice[index] << "\n";
    }

    // ���݂̃_�C�X�̒l��\������֐�
    void printDice() {
        for (int die : dice) {
            cout << die << " ";
        }
        cout << "\n";
    }

    // �e�_�C�X�̖ڂ̏o���񐔂��J�E���g���ăx�N�^�[�ɕϊ�����
    vector<int> getDiceCounts() {
        vector<int> counts(6, 0);
        for (int die : dice) {
            counts[die - 1]++;
        }
        return counts;
    }

    // �_�C�X�̖ڂ̃J�E���g�ƑI�����ꂽ���Ɋ�Â��ē��_���v�Z����֐�
    int calculateScore(const vector<int>& counts, const string& category) {
        if (category == "1�̖�") {
            return counts[0] * 1;
        }
        else if (category == "2�̖�") {
            return counts[1] * 2;
        }
        else if (category == "3�̖�") {
            return counts[2] * 3;
        }
        else if (category == "4�̖�") {
            return counts[3] * 4;
        }
        else if (category == "5�̖�") {
            return counts[4] * 5;
        }
        else if (category == "6�̖�") {
            return counts[5] * 6;
        }
        else if (category == "3�J�[�h") {
            for (int count : counts) {
                if (count >= 3) return accumulate(dice.begin(), dice.end(), 0);
            }
        }
        else if (category == "4�J�[�h") {
            for (int count : counts) {
                if (count >= 4) return accumulate(dice.begin(), dice.end(), 0);
            }
        }
        else if (category == "�t���n�E�X") {
            if (find(counts.begin(), counts.end(), 3) != counts.end() &&
                find(counts.begin(), counts.end(), 2) != counts.end()) {
                return 25;
            }
        }
        else if (category == "�������X�g���[�g") {
            if ((counts[0] && counts[1] && counts[2] && counts[3]) ||
                (counts[1] && counts[2] && counts[3] && counts[4]) ||
                (counts[2] && counts[3] && counts[4] && counts[5])) {
                return 30;
            }
        }
        else if (category == "�傫���X�g���[�g") {
            if ((counts[0] && counts[1] && counts[2] && counts[3] && counts[4]) ||
                (counts[1] && counts[2] && counts[3] && counts[4] && counts[5])) {
                return 40;
            }
        }
        else if (category == "���b�c�B�\") {
            if (find(counts.begin(), counts.end(), 5) != counts.end()) {
                return 50;
            }
        }
        else if (category == "�`�����X") {
            return accumulate(dice.begin(), dice.end(), 0);
        }
        return 0;
    }

    // 13�^�[���ڂ܂ŃQ�[���̃��[�v
    void play() {
        cout << "���b�c ���b�c�B�\!\n";
        vector<string> categories = { "1�̖�", "2�̖�", "3�̖�", "4�̖�", "5�̖�", "6�̖�", "3�J�[�h",
                                     "4�J�[�h", "�t���n�E�X", "�������X�g���[�g", "�傫���X�g���[�g",
                                     "���b�c�B�\", "�`�����X" };

        for (int i = 1; i <= 13; ++i) {
            cout << i << "�^�[����\n\n";
            rollDice();
            printDice();

            for (int rerollCount = 1; rerollCount <= 2; ++rerollCount) {
                vector<int> diceToReroll;
                int input;
                cout << "�_�C�X��U�蒼���܂����H\n�U�蒼���ꍇ�́A���Ԗڂ̃_�C�X��U�蒼�����ԍ�(1~5)�œ��͂��A�U�蒼���Ȃ��ꍇ��0����͂��Ă��������B\n�i�����Ɛ����̊Ԃ̓X�y�[�X�ŋ�؂��Ă��������j [�U�蒼���� " << rerollCount << " / 2]: ";

                // �U�蒼���_�C�X��ǂݍ���
                while (cin >> input) {
                    if (input == 0) break;
                    if (input >= 1 && input <= 5) {
                        diceToReroll.push_back(input);
                    }
                    else {
                        cout << "�����ȓ��͂ł��B1~5�̐�������͂��Ă��������B\n";
                        continue;
                    }
                    if (cin.peek() == '\n') break;
                }

                if (cin.fail()) {
                    cout << "�����ȓ��͂ł��B��������͂��Ă��������B\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    rerollCount--; // �����ȓ��͂̂��ߐU�蒼���񐔂����ɖ߂�
                    continue;
                }

                // �I�����ꂽ�_�C�X��U�蒼��
                for (int dieNum : diceToReroll) {
                    rerollDice(dieNum - 1);
                }

                printDice();
            }

            // �e�_�C�X�̖ڂ̃J�E���g���擾
            vector<int> counts = getDiceCounts();
            cout << "����I�����Ă�������: \n";
            for (size_t j = 0; j < categories.size(); ++j) {
                if (categoryScores[j] == -1) {
                    int potentialScore = calculateScore(counts, categories[j]);
                    cout << j + 1 << ": " << categories[j] << " (���_: " << potentialScore << ")\n";
                }
            }

            // ����I������
            int categoryIndex;
            while (true) {
                cin >> categoryIndex;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "�����ȓ��͂ł��B��������͂��Ă��������B\n";
                    continue;
                }

                if (categoryIndex < 1 || categoryIndex > 13 || categoryScores[categoryIndex - 1] != -1) {
                    cout << "�����ȓ��͂܂��͊��ɑI�����ꂽ���ł��B�ēx����I�����Ă��������B\n";
                }
                else {
                    break;
                }
            }

            categoryIndex--;
            int score = calculateScore(counts, categories[categoryIndex]);
            categoryScores[categoryIndex] = score;
            cout << "���_: " << score << "\n";
            totalScore += score;
            // �㕔�̖��̃X�R�A���X�V
            if (categoryIndex < 6) {
                upperSectionScore += score;
            }
        }

        // �㕔�̖��̃X�R�A��63�_�ȏ�̏ꍇ�A�{�[�i�X��ǉ�
        if (upperSectionScore >= 63) {
            cout << "�㕔�̖��̃X�R�A��63�_�ȏ�ł��I�{�[�i�X +35�_\n";
            totalScore += 35;
        }

        cout << "�Q�[���I���I�ŏI���_: " << totalScore << "\n";
    }
};

int main() {
    DiceGame game;
    game.play();
    return 0;
}
