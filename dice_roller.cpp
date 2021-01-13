#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>

// Function declarations

int  main            ( int argc, char *argv[]                                                               );
void parse_options   ( std::vector<std::string> user_arguments, int &num_dice, int &die_type, int &modifier );
void interactive     ( int argc, char *argv[]                                                               );
int  handle_input    ( std::string &input, bool &run                                                        );
void roll_dice       ( int num_dice, int die_type, int modifier                                             );
void print_help_text ( char *argv[])                                                                        ;

// Enum for error codes
enum Errors {
    INVALID_NUMBER_DIE,
    INVALID_DIE_TYPE,
    INVALID_MODIFIER,
    UNABLE_CONVERT_NUM_DIE,
    UNABLE_CONVERT_DIE_TYPE,
    UNABLE_CONVERT_MODIFIER
};

std::string error_names[6] = {
    "INVALID_NUMBER_DIE",
    "INVALID_DIE_TYPE",
    "INVALID_MODIFIER",
    "UNABLE_CONVERT_NUM_DIE",
    "UNABLE_CONVERT_DIE_TYPE",
    "UNABLE_CONVERT_MODIFIER"
};

int main (int argc, char *argv[]) {
    int num_dice = 0;
    int die_type = 0;
    int modifier = 0;

    if ( argc == 1 ) {
        // No arguments given
        // start interactive mode
        interactive(argc, argv);
        return 0;
    }
    else {
        // Parse command line options
        try {
            std::vector<std::string> user_arguments;
            for ( int i = 1; i < argc; i++ ) {
                user_arguments.push_back(argv[i]);
            }
            parse_options(user_arguments, num_dice, die_type, modifier);
        }
        catch ( Errors e ) {
            switch(e) {
                case INVALID_NUMBER_DIE:
                    std::cout << "[" << error_names[e] << "] " << "Invalid number of dice given" << std::endl;
                    break;
                case INVALID_DIE_TYPE:
                    std::cout << "[" << error_names[e] << "] " << "Invalid die type given" << std::endl;
                    break;
                case INVALID_MODIFIER:
                    std::cout << "[" << error_names[e] << "] " << "Invalid modifier given" << std::endl;
                    break;
                case UNABLE_CONVERT_NUM_DIE:
                    std::cout << "[" << error_names[e] << "] " << "Unable to convert the number of dice given into a number" << std::endl;
                    break;
                case UNABLE_CONVERT_DIE_TYPE:
                    std::cout << "[" << error_names[e] << "] " << "Unable to convert the type of die given into a number" << std::endl;
                    break;
                case UNABLE_CONVERT_MODIFIER:
                    std::cout << "[" << error_names[e] << "] " << "Unable to convert the modifier given into a number" << std::endl;
                    break;
            }
            print_help_text(argv);

            return 1;
        }
        catch (...) {
            std::cout << "Unknown exception" << std::endl;

            return 1;
        }

        // Calculate roll results
        roll_dice(num_dice, die_type, modifier);
    }

    return 0;
}

void parse_options (std::vector<std::string> user_arguments, int &num_dice, int &die_type, int &modifier) {
    std::string num_dice_str;
    std::string die_type_str;
    std::string modifier_str;

    for ( int i = 0; i < user_arguments.size(); i++ ) {
        std::string a = user_arguments[i];
        if ( user_arguments[i] == "--help" || user_arguments[i] == "--h" ) {
            throw -1;
        }
        for ( int j = 0; j < a.length(); j++ ) {
            if ( i < 1 ) {
                if ( isdigit(a[j]) && j < 1 ) {
                    // First character should be a number
                    num_dice_str = std::string(a.begin(), std::find(a.begin(), a.end(), 'd'));
                    j = std::distance(a.begin(), std::find(a.begin(), a.end(), 'd'));
                    continue;
                }
                else if ( j < 1 ) {
                    Errors e = INVALID_NUMBER_DIE;
                    throw e;
                }

                if ( isdigit(a[j]) ) {
                    std::string::iterator it = a.begin();
                    std::advance(it, j);
                    die_type_str = std::string(it, a.end());
                    break;
                }
                else {
                    Errors e = INVALID_DIE_TYPE;
                    throw e;
                }
            }
            else {
                if ( a[j] == '+' ) {
                    // Modifier is postive
                    modifier_str = a[j];
                }
                else if ( a[j] == '-' ) {
                    // Modifier is negative
                    modifier_str = a[j];
                }
                else if ( isdigit(a[j]) ) {
                    modifier_str += a[j];
                }
                else {
                    std::cerr << "# There was an error parsing the modifier string: [" << a << "]" << std::endl;
                    Errors e = INVALID_MODIFIER;
                    throw e;
                }
            }
        }
    }

    try {
        num_dice = std::stoi(num_dice_str);
    }
    catch (...) {
        Errors e = UNABLE_CONVERT_NUM_DIE;
        throw e;
    }
    try {
        die_type = std::stoi(die_type_str);
    }
    catch (...) {
        Errors e = UNABLE_CONVERT_DIE_TYPE;
        throw e;
    }
    if ( modifier_str != "" ) {
        try {
            modifier = std::stoi(modifier_str);
        }
        catch (...) {
            Errors e = UNABLE_CONVERT_MODIFIER;
            throw e;
        }
    }

    return;
}

void interactive (int argc, char *argv[]) {
    bool run = true;
    std::cout << "Welcome to the dice roller!" << std::endl;
    std::cout << "Follow the prompts to roll as many dice as you want." << std::endl;
    std::cout << "To stop the program type in either 'q', 'quit', or press ctrl + d at any time." << std::endl << std::endl;

    while (run) {
        int num_dice = 0;
        int die_type = 0;
        int modifier = 0;
        std::string num_dice_str;
        std::cout << "Enter in the number of dice to roll: ";
        if ( !handle_input(num_dice_str, run) ) {
            continue;
        }

        std::string die_type_str;
        std::cout << "Enter in the type of die to roll: ";
        if ( !handle_input(die_type_str, run) ) {
            continue;
        }

        std::string modifier_str;
        std::cout << "Enter in the modifier: ";
        if ( !handle_input(modifier_str, run) ) {
            continue;
        }

        std::vector<std::string> user_arguments;
        if ( std::find(die_type_str.begin(), die_type_str.end(), 'd') != die_type_str.end() ) {
            // User gave us a die type that looks like "d6"
            user_arguments.push_back( num_dice_str + die_type_str );
        }
        else {
            // User gave us just a number for the die type
            user_arguments.push_back( num_dice_str + 'd' + die_type_str );
        }
        user_arguments.push_back( modifier_str );

        try {
            parse_options(user_arguments, num_dice, die_type, modifier);
            roll_dice(num_dice, die_type, modifier);
            std::cout << std::endl;
        }
        catch (Errors e) {
            switch(e) {
                case INVALID_NUMBER_DIE:
                    std::cout << "[" << error_names[e] << "] " << "Invalid number of dice given" << std::endl;
                    break;
                case INVALID_DIE_TYPE:
                    std::cout << "[" << error_names[e] << "] " << "Invalid die type given" << std::endl;
                    break;
                case INVALID_MODIFIER:
                    std::cout << "[" << error_names[e] << "] " << "Invalid modifier given" << std::endl;
                    break;
                case UNABLE_CONVERT_NUM_DIE:
                    std::cout << "[" << error_names[e] << "] " << "Unable to convert the number of dice given into a number" << std::endl;
                    break;
                case UNABLE_CONVERT_DIE_TYPE:
                    std::cout << "[" << error_names[e] << "] " << "Unable to convert the type of die given into a number" << std::endl;
                    break;
                case UNABLE_CONVERT_MODIFIER:
                    std::cout << "[" << error_names[e] << "] " << "Unable to convert the modifier given into a number" << std::endl;
                    break;
            }
        }
    }

    return;
}

int handle_input (std::string &input, bool &run) {
    if ( ! (std::cin >> input) ) {
        std::cout << std::endl << "Goodbye!" << std::endl;
        run = false;

        return 0;
    }
    else if ( input == "q" || input == "quit" ) {
        std::cout << std::endl << "Goodbye!" << std::endl;
        run = false;

        return 0;
    }

    return 1;
}


void roll_dice(int num_dice, int die_type, int modifier) {
    std::random_device rd;
    std::vector<int> results;

    // seed value is designed specifically to make initialization
    // parameters of std::mt19937 (instance of std::mersenne_twister_engine<>)
    // different across executions of application
    std::mt19937::result_type seed = rd() ^ ( (std::mt19937::result_type) std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + (std::mt19937::result_type) std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() );
    std::mt19937 gen(seed);

    for ( int i = 0; i < num_dice; i++ ) {
        std::mt19937::result_type n;
        // reject readings that would make n%6 non-uniformly distributed
        while( ( n = gen() ) > std::mt19937::max() - ( std::mt19937::max() - 5 ) % 6 )
        {
            /* bad value retrieved so get next one */
        }

        int result = (n % die_type) + 1;
        results.push_back(result);
    }

    // for ( auto &r : results ) {
    int total = 0;
    for ( int i = 0; i < results.size(); i++ ) {
        std::cout << "Roll [" << i + 1 << "]: " << results[i] << std::endl;
        total += results[i];
    }
    total += modifier;
    std::cout << std::endl << "Total: " << total << std::endl;

    return;
}

void print_help_text (char *argv[]) {

    std::string prog_name = argv[0];

    printf("Usage: %s: [--help|h] [die] [modifier]\n", prog_name.c_str());
    printf("Examples:\n");
    printf("\t%s 1d6\n", prog_name.c_str());
    printf("\t\tRolls one d6 and prints the result\n");
    printf("\t%s 2d6 +2\n", prog_name.c_str());
    printf("\t\tRolls two d6, then adds two and prints the result\n");
    printf("\t%s d20\n", prog_name.c_str());
    printf("\t\tRolls one d20 and prints the result\n");
    printf("\t%s\n", prog_name.c_str());
    printf("\t\tRuns the program in interactive mode\n");

    return;
}
