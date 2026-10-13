#include <LITW.hpp>
#include <math.h>
#include <string>
#include <vector>

WindowManager wm;

class TringleSolver : public Window {
    int lastHeightForTextBoxes = 2;
    double a = 0;
    double b = 0;
    double c = 0;
    TextBox* NormalResult = nullptr;
    TextBox* MissingAResult = nullptr;
    std::vector<std::string> sad = { };

public:
    TringleSolver() : Window("Triangle Solver", 45, 14) {
        auto& vbox = Add<VerticalContainer>(2, 2);
        vbox.Add<Label>("Triangle Solver");
        auto& hbox = vbox.Add<HorizontalContainer>();
        vbox.Add<Label>("Normal:");
        hbox.Add<Label>("A:");
        hbox.Add<NumberInput<double>>(5, &a);
        hbox.Add<Label>("B:");
        hbox.Add<NumberInput<double>>(5, &b);
        hbox.Add<Button>("Calculate", [this]() {
            NormalResult->text.clear();
            NormalResult->text.push_back(std::to_string(a) + "² + " + std::to_string(b) + "² = x²");
            double aSq = a * a;
            double bSq = b * b;
            NormalResult->text.push_back(std::to_string(aSq) + " + " + std::to_string(bSq) + " = x²");
            double sum = aSq + bSq;
            NormalResult->text.push_back(std::to_string(sum) + " = x² / √" + std::to_string(sum));
            double result = std::sqrt(sum);
            NormalResult->text.push_back("x = " + std::to_string(result));
            });
        NormalResult = &vbox.Add<TextBox>(sad);
        vbox.Add<HorizontalSeparator>();
        vbox.Add<Label>("Missing A:");
        auto& hbox2 = vbox.Add<HorizontalContainer>();
        hbox2.Add<Label>("B:");
        hbox2.Add<NumberInput<double>>(5, &b);
        hbox2.Add<Label>("C:");
        hbox2.Add<NumberInput<double>>(5, &c);
        hbox2.Add<Button>("Calculate", [this]() {
            MissingAResult->text.clear();
            MissingAResult->text.push_back("x² + " + std::to_string(b) + "² = " + std::to_string(c) + "²");
            double bSq = b * b;
            double cSq = c * c;
            MissingAResult->text.push_back("x² + " + std::to_string(bSq) + " = " + std::to_string(cSq));
            double diff = cSq - bSq;
            MissingAResult->text.push_back("x² = " + std::to_string(diff) + " / √" + std::to_string(diff));
            if (diff < 0) {
                MissingAResult->text.push_back("Error: b cannot be greater than c");
            }
            else {
                double result = std::sqrt(diff);
                MissingAResult->text.push_back("x = " + std::to_string(result));
            }
            });
        MissingAResult = &vbox.Add<TextBox>(sad);
        vbox.Add<Button>("Exit", [this]() {
            wm.exit(0);
            });
    }
    void Draw(std::ostream& buffer) override {
        int baseHeight = 14;
        height = baseHeight + (NormalResult->GetHeight() + MissingAResult->GetHeight());
        Window::Draw(buffer);
    }
};

int main() {
    auto start = startmenu<StartMenuWindow>(&wm);
    start->AddItem<TringleSolver>("Triangle Solver");
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.Run();
    wm.exit(1);
    return 0;
}
