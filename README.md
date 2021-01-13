# Dice Roller

- [Dice Roller](#dice-roller)
- [Synopsis](#synopsis)
- [How to use](#how-to-use)
  - [Interactive](#interactive)
    - [Examples](#examples)
  - [Non-interactive mode](#non-interactive-mode)
    - [Examples](#examples-1)
  - [Other options](#other-options)

# Synopsis

This is a simple programming for rolling a given number of dice and potentially adding a modifier to the results. Easy to use, random (in a good way), and helpful when physical dice aren't at your fingertips.

# How to use

There are two ways to use this program. Interactively and non-interactively.

## Interactive

To start the program in interactive mode, simple run it without giving it any arguments. This will start a prompt asking you for details on what to roll.

### Examples
```
% ./dice_roller
Welcome to the dice roller!
Follow the prompts to roll as many dice as you want.
To stop the program type in either 'q', 'quit', or press ctrl + d at any time.

Enter in the number of dice to roll:
```

From there you would enter in a number:

```
Enter in the number of dice to roll: 1
Enter in the type of die to roll:
```

Then the type of die to roll (e.g. `6` or `d6`):

```
Enter in the type of die to roll: 6
Enter in the modifier:
```

Lastly, enter in any modifier you want to apply to the result:

```
Enter in the modifier: +3
```

Once you enter in the modifier, the program will calculate the rolls and print out the results for each one, as well as a total that includes the modifier given:

```
Roll [1]: 2

Total: 5
```

This repeats until you either hit `ctrl + d`, or submit either `q` or `quit` for any of the options.

## Non-interactive mode

To run this mode, simply run the program with two arguments, one for the number and type of die to roll, and one for the modifier.

The first argument needs to be in the format: `{number}d{number}`, for example a single d6 would be `1d6`.

The second argument needs to be in the format: `+{number}` or `-{number}`, for example a modifier of a positive 3 would be `+3`.
The second argument is optional.

### Examples

To roll a single 6-sided die without a modifier you would run the program with the argument of `1d6`:

```
% ./dice_roller 1d6
Roll [1]: 3

Total: 3
```

To roll a single 6-sided die with a modifier you would run the program with the argument of `1d6` and a second argument with the modifier, for example `+2`:

```
% ./dice_roller 1d6 +2
Roll [1]: 4

Total: 6
```

## Other options

If you don't remember how to use the program, simply run it and give it either `-h` or `--help` as the option and it will print out a usage message describing how to use it.

```
% ./dice_roller --help
Usage: ./dice_roller: [--help|h] [die] [modifier]
Examples:
	./dice_roller 1d6
		Rolls one d6 and prints the result
	./dice_roller 2d6 +2
		Rolls two d6, then adds two and prints the result
	./dice_roller d20
		Rolls one d20 and prints the result
	./dice_roller
		Runs the program in interactive mode
```