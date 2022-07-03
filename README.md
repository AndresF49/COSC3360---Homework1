## For more assignment details, check pdf

For this assignment, you will create a parallel fixed-length code decompressor using the tools we learned in class to create multiple
processes and threads.

Read the contents of the input file using input redirection (STDIN). The format of the input file is as follows:
- An integer value representing the number of symbols in the alphabet
- n lines (where n is the number of symbols in the alphabet) with a char representing the value of the symbol and an integer value representing the symbol's code (in decimal notation).
- A string representing the compressed message (sequence of bits).

Calculate the number of bits of the fixed-length codes based on the value of the greatest base 10 code in the alphabet. For
decimal values greater than zero, you can use the following formula to calculate the number of bits of the fixed-length codes:
ceiling(log_2(greatest_base_10_code_in_the_alphabet +1)).

Create n child processes or threads (where n is the number of symbols in the alphabet). Each child process or thread executes
the following tasks:
- Determines the binary representation of the symbol's code.
- Determines the frequency of the symbol in the compressed message.

Create m child processes or threads (where m is the number of characters in the decompressed message) to determine each
character of the decompressed message.

Print the information about the alphabet and the decompressed message.