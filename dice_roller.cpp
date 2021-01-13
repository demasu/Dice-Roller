#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>

// Function declarations

void print_help_text(char *argv[]);
void parse_options(int argc, char *argv[], int &num_dice, int &die_type, int &modifier);
void roll_dice(int num_dice, int die_type, int modifier);

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
        // TODO: create interactive mode
        print_help_text(argv);
        return 1;
    }
    else {
        // Parse command line options
        try {
            parse_options(argc, argv, num_dice, die_type, modifier);
        }
        catch ( int e ) {
            switch(e) {
                case 0:
                    std::cout << "[" << error_names[e] << "] " << "Invalid number of dice given" << std::endl;
                    break;
                case 1:
                    std::cout << "[" << error_names[e] << "] " << "Invalid die type given" << std::endl;
                    break;
                case 2:
                    std::cout << "[" << error_names[e] << "] " << "Invalid modifier given" << std::endl;
                    break;
                case 3:
                    std::cout << "[" << error_names[e] << "] " << "Unable to convert the number of dice given into a number" << std::endl;
                    break;
                case 4:
                    std::cout << "[" << error_names[e] << "] " << "Unable to convert the type of die given into a number" << std::endl;
                    break;
                case 5:
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

void parse_options (int argc, char *argv[], int &num_dice, int &die_type, int &modifier) {
    std::vector<std::string> all_args(argv, argv + argc);
    std::string num_dice_str;
    std::string die_type_str;
    std::string modifier_str;

    for ( int i = 0; i < all_args.size(); i++ ) {
        if ( all_args[i] == "--help" || all_args[i] == "--h" ) {
            throw -1;
        }
        if ( all_args[i] == argv[0] ) {
            continue; // Skip the program name
        }
        else {
            std::string a = all_args[i];
            for ( int j = 0; j < a.length(); j++ ) {
                if ( i < 2 ) {
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
                        Errors e = INVALID_MODIFIER;
                        throw e;
                    }
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

    return;
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
        std::cout << "Roll [" << i << "]: " << results[i] << std::endl;
        total += results[i];
    }
    total += modifier;
    std::cout << std::endl << "Total: " << total << std::endl;

    return;
}
