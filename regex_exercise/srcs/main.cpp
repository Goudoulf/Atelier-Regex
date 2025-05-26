#include "../include/json.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <regex>

using namespace ftxui;

struct Exercise {
    std::vector<std::string> expected;
    std::string description;
    std::string text;
	bool completed = false;
};

struct Token {
    std::vector<Exercise> exercises;
    std::string token;
    std::string description;
    bool completed = false;
};

std::vector<Token> tokens;

void loadExercises(){
    std::ifstream file("ex.json");
    nlohmann::json data;
    file >> data;

    for (const auto& item : data){
        Token token;
        token.token = item["token"];
        token.description = item["description"];

        for (const auto& subItem : item["exercises"]){
            Exercise exercise;
            exercise.description = subItem["description"];
            exercise.text = subItem["text"];
            exercise.expected = subItem["expected"];
            token.exercises.push_back(exercise);
        }
        tokens.push_back(token);
    }
}

std::vector<std::string> checkRegexInput(std::string text, std::string input){
	std::vector<std::string> results;
	
	try {
		std::regex re(input);
		std::smatch matches;
	
		auto start = text.cbegin();
		auto end = text.cend();
	
		while (std::regex_search(start, end, matches, re)) {
			results.push_back(matches.str());
			start = matches.suffix().first;
		}
	} catch (const std::regex_error& e) {

	}
    return results;
}

std::string normalize_tabs(const std::string& input, int tab_width = 4) {
    std::string result;
    for (char c : input) {
        if (c == '\t')
            result += std::string(tab_width, ' ');
        else
            result += c;
    }
    return result;
}

std::string visualize_whitespace(const std::string& input) {
    std::string output;
    for (char c : input) {
        switch (c) {
            // case ' ': output += "\' \'"; break;
            case '\t': output += "\\t"; break;
            case '\n': output += "\\n"; break;
            default: output += c; break;
        }
    }
    return output;
}

void showExerciseScreen(std::vector<Token>& tokens, int start_index = 0) {
    int currentTokenIndex = start_index;

    while (currentTokenIndex < tokens.size()) {
        Token& token = tokens[currentTokenIndex];
        auto screen = ScreenInteractive::Fullscreen();

        std::string input_text;
        std::vector<std::string> results;
        int currentExerciseIndex = 0;
        bool returnToMenu = false;
        bool nextTokenRequested = false;
        bool prevTokenRequested = false;

        auto input = Input(&input_text, "Regex input...");

        input |= CatchEvent([&](Event event) {
            if (event == Event::Return) {
                if (input_text.empty()) return false;
                results = checkRegexInput(token.exercises[currentExerciseIndex].text, input_text);
                token.exercises[currentExerciseIndex].completed = true;

                if (results.empty()) {
                    token.exercises[currentExerciseIndex].completed = false;
                    results.push_back("❌ Regex is invalid...");
                    return true;
                }

                for (size_t i = 0; i < std::max(token.exercises[currentExerciseIndex].expected.size(), results.size()); ++i) {
                    if (i < results.size() && i >= token.exercises[currentExerciseIndex].expected.size()) {
                        token.exercises[currentExerciseIndex].completed = false;
                        results[i] += " ❌";
                    } else if (i >= results.size() && i < token.exercises[currentExerciseIndex].expected.size()) {
                        token.exercises[currentExerciseIndex].completed = false;
                        results.push_back("❌ Expected more match...");
                        return true;
                    } else {
						results[i] += results[i] == token.exercises[currentExerciseIndex].expected[i] ? "\' ✅" : "\' ❌";
						results[i] = "\'" + results[i];
                    }
                }
                return true;
            }
            return false;
        });

        auto next_button = Button("Next →", [&] {
            if (currentExerciseIndex + 1 < token.exercises.size()) {
                currentExerciseIndex++;
                input_text.clear();
                results.clear();
            }
        });

        auto prev_button = Button("← Previous", [&] {
            if (currentExerciseIndex > 0) {
                currentExerciseIndex--;
                input_text.clear();
                results.clear();
            }
        });

        auto next_token_button = Button("Next Token →", [&] {
            nextTokenRequested = true;
            screen.Exit();
        });

        auto prev_token_button = Button("← Previous Token", [&] {
            prevTokenRequested = true;
            screen.Exit();
        });

        auto back_button = Button("← BACK TO MAIN MENU", [&] {
            returnToMenu = true;
            screen.Exit();
        });

        auto container = Container::Vertical({input, prev_button, next_button, next_token_button, prev_token_button, back_button});

        auto main_component = Renderer(container, [&] {
            std::vector<Element> result_elements;
            for (const auto& res : results)
                result_elements.push_back(text(visualize_whitespace(res)) | color(Color::Yellow) | bold);

            return vbox({
                text("Token: " + token.token) | bold | center,
                separator(),
                window(text(" Token Description "), paragraph(token.description)),
                separator(),
                window(text(" Exercise " + std::to_string(currentExerciseIndex + 1)),
                    vbox({
                        paragraph(normalize_tabs(token.exercises[currentExerciseIndex].description)),
                        separator(),
                        paragraph(normalize_tabs(token.exercises[currentExerciseIndex].text)) | color(Color::Green),
                    })),
                separator(),
                vbox({input->Render(), separator(), text("Press [Enter] to validate input")}) | border,
                vbox(result_elements),
                filler(),
                hbox({prev_button->Render(), filler(), next_button->Render()}),
                hbox({prev_token_button->Render(), filler(), next_token_button->Render()}),
                back_button->Render() | center
            });
        });

        screen.Loop(main_component);

        if (returnToMenu) break;
        if (nextTokenRequested && currentTokenIndex + 1 < tokens.size()) currentTokenIndex++;
        else if (prevTokenRequested && currentTokenIndex > 0) currentTokenIndex--;
        else break;
    }
}

int main() {
    loadExercises();
    while (true) {
        auto screen = ScreenInteractive::Fullscreen();

        std::vector<std::string> token_labels;
        for (size_t i = 0; i < tokens.size(); ++i)
            token_labels.push_back(std::to_string(i + 1) + ". " + tokens[i].token);

        int selected_index = 0;
        auto menu = Menu(&token_labels, &selected_index);

        bool launch_exercise = false;
        menu |= CatchEvent([&](Event event) {
			if (event == Event::Return) {
				launch_exercise = true;
				screen.Exit();
				return true;
			}
			if (event == Event::Escape) {
				screen.Exit();
				return true;
			}
			return false;
		});

        auto main_component = Renderer(menu, [&] {
            return vbox({
                text("=== Select a Token ===") | bold | center,
                separator(),
                menu->Render(),
				separator(),
				text("Press [Enter] to select, [Esc] to quit.") | center
            }) | border | center;
        });

        screen.Loop(main_component);

        if (launch_exercise) {
            showExerciseScreen(tokens, selected_index);
        } else {
            break;
        }
    }
    return 0;
}

