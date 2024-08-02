#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <string>
using namespace std;

class DiceGame {
private:
    vector<int> dice;  // 現在のダイスの値を格納する
    vector<int> categoryScores;  // 各役の得点を格納する
    int totalScore;  // 合計得点を格納する変数
    int upperSectionScore;  // 「1の目」から「6の目」（上部の役）の合計得点を格納する変数
    random_device engine;  // 乱数の生成

public:
    DiceGame() : dice(5), categoryScores(13, -1), totalScore(0), upperSectionScore(0) {}

    // minからmaxまでの乱数を生成する関数
    int getRandomInt(int min, int max) {
        uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }

    // 5つのダイスを振る関数
    void rollDice() {
        for (int& die : dice) {
            die = getRandomInt(1, 6);
        }
    }

    // 特定のダイスを振り直す関数
    void rerollDice(int index) {
        dice[index] = getRandomInt(1, 6);
        cout << "ダイスを振り直しました: " << dice[index] << "\n";
    }

    // 現在のダイスの値を表示する関数
    void printDice() {
        for (int die : dice) {
            cout << die << " ";
        }
        cout << "\n";
    }

    // 各ダイスの目の出現回数をカウントしてベクターに変換する
    vector<int> getDiceCounts() {
        vector<int> counts(6, 0);
        for (int die : dice) {
            counts[die - 1]++;
        }
        return counts;
    }

    // ダイスの目のカウントと選択された役に基づいて得点を計算する関数
    int calculateScore(const vector<int>& counts, const string& category) {
        if (category == "1の目") {
            return counts[0] * 1;
        }
        else if (category == "2の目") {
            return counts[1] * 2;
        }
        else if (category == "3の目") {
            return counts[2] * 3;
        }
        else if (category == "4の目") {
            return counts[3] * 4;
        }
        else if (category == "5の目") {
            return counts[4] * 5;
        }
        else if (category == "6の目") {
            return counts[5] * 6;
        }
        else if (category == "3カード") {
            for (int count : counts) {
                if (count >= 3) return accumulate(dice.begin(), dice.end(), 0);
            }
        }
        else if (category == "4カード") {
            for (int count : counts) {
                if (count >= 4) return accumulate(dice.begin(), dice.end(), 0);
            }
        }
        else if (category == "フルハウス") {
            if (find(counts.begin(), counts.end(), 3) != counts.end() &&
                find(counts.begin(), counts.end(), 2) != counts.end()) {
                return 25;
            }
        }
        else if (category == "小さいストレート") {
            if ((counts[0] && counts[1] && counts[2] && counts[3]) ||
                (counts[1] && counts[2] && counts[3] && counts[4]) ||
                (counts[2] && counts[3] && counts[4] && counts[5])) {
                return 30;
            }
        }
        else if (category == "大きいストレート") {
            if ((counts[0] && counts[1] && counts[2] && counts[3] && counts[4]) ||
                (counts[1] && counts[2] && counts[3] && counts[4] && counts[5])) {
                return 40;
            }
        }
        else if (category == "ヤッツィ―") {
            if (find(counts.begin(), counts.end(), 5) != counts.end()) {
                return 50;
            }
        }
        else if (category == "チャンス") {
            return accumulate(dice.begin(), dice.end(), 0);
        }
        return 0;
    }

    // 13ターン目までゲームのループ
    void play() {
        cout << "レッツ ヤッツィ―!\n";
        vector<string> categories = { "1の目", "2の目", "3の目", "4の目", "5の目", "6の目", "3カード",
                                     "4カード", "フルハウス", "小さいストレート", "大きいストレート",
                                     "ヤッツィ―", "チャンス" };

        for (int i = 1; i <= 13; ++i) {
            cout << i << "ターン目\n\n";
            rollDice();
            printDice();

            for (int rerollCount = 1; rerollCount <= 2; ++rerollCount) {
                vector<int> diceToReroll;
                int input;
                cout << "ダイスを振り直しますか？\n振り直す場合は、何番目のダイスを振り直すか番号(1~5)で入力し、振り直さない場合は0を入力してください。\n（数字と数字の間はスペースで区切ってください） [振り直し回数 " << rerollCount << " / 2]: ";

                // 振り直すダイスを読み込む
                while (cin >> input) {
                    if (input == 0) break;
                    if (input >= 1 && input <= 5) {
                        diceToReroll.push_back(input);
                    }
                    else {
                        cout << "無効な入力です。1~5の数字を入力してください。\n";
                        continue;
                    }
                    if (cin.peek() == '\n') break;
                }

                if (cin.fail()) {
                    cout << "無効な入力です。数字を入力してください。\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    rerollCount--; // 無効な入力のため振り直し回数を元に戻す
                    continue;
                }

                // 選択されたダイスを振り直す
                for (int dieNum : diceToReroll) {
                    rerollDice(dieNum - 1);
                }

                printDice();
            }

            // 各ダイスの目のカウントを取得
            vector<int> counts = getDiceCounts();
            cout << "役を選択してください: \n";
            for (size_t j = 0; j < categories.size(); ++j) {
                if (categoryScores[j] == -1) {
                    int potentialScore = calculateScore(counts, categories[j]);
                    cout << j + 1 << ": " << categories[j] << " (得点: " << potentialScore << ")\n";
                }
            }

            // 役を選択する
            int categoryIndex;
            while (true) {
                cin >> categoryIndex;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "無効な入力です。数字を入力してください。\n";
                    continue;
                }

                if (categoryIndex < 1 || categoryIndex > 13 || categoryScores[categoryIndex - 1] != -1) {
                    cout << "無効な入力または既に選択された役です。再度役を選択してください。\n";
                }
                else {
                    break;
                }
            }

            categoryIndex--;
            int score = calculateScore(counts, categories[categoryIndex]);
            categoryScores[categoryIndex] = score;
            cout << "得点: " << score << "\n";
            totalScore += score;
            // 上部の役のスコアを更新
            if (categoryIndex < 6) {
                upperSectionScore += score;
            }
        }

        // 上部の役のスコアが63点以上の場合、ボーナスを追加
        if (upperSectionScore >= 63) {
            cout << "上部の役のスコアが63点以上です！ボーナス +35点\n";
            totalScore += 35;
        }

        cout << "ゲーム終了！最終得点: " << totalScore << "\n";
    }
};

int main() {
    DiceGame game;
    game.play();
    return 0;
}
