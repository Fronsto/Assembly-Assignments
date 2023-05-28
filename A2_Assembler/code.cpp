// Name : Anindya Vijayvargeeya
// Roll : 200101015

#include <bits/stdc++.h>

using namespace std;

//file streams for file I/O management
ifstream fin;
ofstream fout;

// OpCode table
unordered_map < string, string > OPTAB;
// Symbol Table
unordered_map < string, string > SYMTAB;

// to store file names
string input_file_name;
string intermediate_file_name;
string object_file_name;

string start_address;  //stores the start address
string first_exec_address; //stores the first executable address
string program_name;  //stores the name of program
string program_length; //stores the length of program

long long LOCCTR; //Location Counter

int line_num; // stores line number from input file

string line; //stores lines read from input file

// following are used for tokenization purposes
vector < string > tokens; //stores line in tokenzied form
string label; //store label separately
string token; //stores individual words

int pass_no = 1; //stores the pass number

//populates Operation Code Table
void setup_optab() {
    OPTAB["ADD"] = "18";
    OPTAB["SUB"] = "1C";
    OPTAB["MUL"] = "20";
    OPTAB["DIV"] = "24";
    OPTAB["COMP"] = "28";
    OPTAB["LDA"] = "00";
    OPTAB["LDX"] = "04";
    OPTAB["LDL"] = "08";
    OPTAB["STA"] = "0C";
    OPTAB["STX"] = "10";
    OPTAB["STL"] = "14";
    OPTAB["LDCH"] = "50";
    OPTAB["STCH"] = "54";
    OPTAB["TD"] = "E0";
    OPTAB["RD"] = "D8";
    OPTAB["WD"] = "DC";

    OPTAB["J"] = "3C";
    OPTAB["JLT"] = "38";
    OPTAB["JEQ"] = "30";
    OPTAB["JGT"] = "34";
    OPTAB["JSUB"] = "48";
    OPTAB["RSUB"] = "4C";
    OPTAB["TIX"] = "2C";

    // we also print the optable to a file named opcodes.txt
    ofstream fout("opcodes.txt");
    for (auto & it: OPTAB) {
        fout << it.first << " " << it.second << "\n";
    }
    fout.close();
}

void print_symtab() {
    ofstream fout("symtab.txt");
    // first string could be of variable lenght, so we use setw to format the output
    fout << setw(8) << left << "SYMBOL" << "\t" << "ADDRESS" << "\n";
    fout << setw(8) << left << "------" << "\t" << "-------" << "\n";
    for (auto & it: SYMTAB) {
        fout << setw(8) << left << it.first << "\t" << it.second << "\n";
    }
    fout.close();
}

// convert integer to string in hexadecimal
template < typename T >
string int_to_hex(T i) {
    stringstream stream;
    stream << hex << uppercase << i;
    return stream.str();
}


// check if given string is a directive
bool is_directive(string tok) {
    return ( tok == "START" || tok == "END" || tok == "BYTE" || tok == "WORD" || tok == "RESB" || tok == "RESW");
}
// check if given string is a valid opcode
bool is_valid_opcode(string& tok) {
    return (OPTAB.find(tok) != OPTAB.end()) || (tok == "BYTE" || tok == "WORD" || tok == "RESB" || tok == "RESW");
}
// check if given string is a label
bool is_label(string tok) {
    return OPTAB.find(tok) == OPTAB.end() && (!is_directive(tok));
}
bool is_comment; // stores if the current line is a comment


// gets input from fin, and tokenizes it
void proc_next_line(){
    bool emp = true;
    while (emp) {
        getline(fin, line);
        line_num++;
        // loop if we get empty line
        for (char& u: line) {
            if (u != ' ') emp = false;
        }
    }
    tokens.clear();
    stringstream ss(line);
    while (ss >> token) {
        tokens.push_back(token);
    }

    if(tokens[0][0] == '.') is_comment = true;
    else is_comment = false;

    if(!is_comment){
        if(pass_no == 2) {
            LOCCTR = stoll(tokens[0], nullptr, 16); // extract locctr value
            tokens.erase(tokens.begin());           // remove the locctr value from tokens
        }
        if(is_label(tokens[0])){
            label = tokens[0];            // extract label 
            tokens.erase(tokens.begin()); // remove label from tokens
        } else {
            label = "";
        }
    }

}

// for printing the object code we need to format the string -

// formats the string to given length by appending characters or truncating
// fmt_string("hello", 10, 0, '0') returns "00000hello"
// fmt_string("hello", 10, 1, '0') returns "hello00000"
string fmt_string(string str, int out_len, int appending_position, char appending_character) {
    int sz = (int)str.length();
    if (sz > out_len)
        str = str.substr(0, out_len);
    else if ( sz < out_len){
        if (appending_position == 0) {
            str.insert(str.begin(), out_len - sz, appending_character);
        } else {
            str.insert(str.end(), out_len - sz, appending_character);
        }
    }
    return str;
}

string error_message;
//handles errors in input file
void handle_error(int pass, string & error_message, int line_num, string error_word = "") {
    fin.close();
    fout.close();
    if (error_word.length())
        cout << error_message << " " << error_word << " ON LINE NUMBER " << line_num << "\n";
    else
        cout << error_message << " ON LINE NUMBER " << line_num << "\n";
    cout << "EXITING PROGRAM\n";
    exit(0);
}


// ██████╗░░█████╗░░██████╗░██████╗  ░░███╗░░
// ██╔══██╗██╔══██╗██╔════╝██╔════╝  ░████║░░
// ██████╔╝███████║╚█████╗░╚█████╗░  ██╔██║░░
// ██╔═══╝░██╔══██║░╚═══██╗░╚═══██╗  ╚═╝██║░░
// ██║░░░░░██║░░██║██████╔╝██████╔╝  ███████╗
// ╚═╝░░░░░╚═╝░░╚═╝╚═════╝░╚═════╝░  ╚══════╝
void pass1() {
    // first open input and output files
    fin.open(input_file_name);
    if (!fin.is_open()) {
        cout << "Error opening input file" << endl;
        exit(0);
    }
    fout.open(intermediate_file_name);
    if (!fout.is_open()) {
        cout << "Error creating intermediate file" << endl;
        exit(0);
    }
    //read first input line
    proc_next_line();
    // loop over initial lines if they are comments
    while (!fin.eof() && is_comment) {
        line = "     " + line;
        fout << line << endl;
        proc_next_line();
    }

    // we initialize LOCCTR based on whether there is a START directive
    if (tokens[0] == "START") {
        //save LABEL in program name
        program_name = label;
        //save #[OPERAND] as starting address
        start_address = tokens[1];

        //initialize LOCCTR to starting address (written in hex)
        LOCCTR = stoll(start_address, nullptr, 16);

        //write line to intermediate file
        line = int_to_hex(LOCCTR) + " " + line;
        fout<< line << endl;

        //read next input line
        proc_next_line();
    }
    //if OPCODE != 'START'
    else {
        program_name = "";      //save empty string in program name
        start_address = "0"; //save 0 as starting address
        LOCCTR = 0;             //initialize LOCCTR to 0
    }

    // loop over the input file, line by line till we see END
    while (tokens[0] != "END") {

        if (is_comment) {
            // instead of printing LOCCTR, we pad with spaces
            int pad_sz = int_to_hex(LOCCTR).size();
            for(int i = 0; i < pad_sz; i++) fout << " ";
        }
        else {
            fout << hex << uppercase << LOCCTR ;

            if (!label.empty()) {
                if(SYMTAB.find(label) != SYMTAB.end()) { 
                    error_message = "DUPLICATE SYMBOL";
                    handle_error(1, error_message, line_num, label);
                }
                SYMTAB[label] = int_to_hex(LOCCTR);
            }

            // if its not a valid opcode
            if(tokens.empty() || (!is_valid_opcode(tokens[0]))) { 
                error_message = "INVALID INSTRUCTION";
                handle_error(1, error_message, line_num);
            }

            //if OPCODE is in OPTAB
            if (OPTAB.find(tokens[0]) != OPTAB.end()) {
                LOCCTR = LOCCTR + 3;
            }
            else if (tokens[0] == "WORD") {
                LOCCTR = LOCCTR + 3;
            }
            else if (tokens[0] == "RESW") {
                string operand = tokens[1];
                LOCCTR = LOCCTR + 3 * stoll(operand);
            }
            else if (tokens[0] == "RESB") {
                string operand = tokens[1];
                LOCCTR = LOCCTR + stoll(operand);
            }
            else if (tokens[0] == "BYTE") {
                string operand = tokens[1];

                //if it is character constant
                if (operand[0] == 'C') {
                    // each character is 1 byte
                    LOCCTR = LOCCTR + operand.length() - 3; // -3 for C' and '
                }
                //if it is hexadecimal constant
                else if (operand[0] == 'X') {
                    // each hexadecimal digit is half a byte
                    int num_digs = operand.length() - 3;
                    LOCCTR = LOCCTR + (num_digs >> 1);
                }
            } 
        }

        fout << " " << line << endl;  //write line to intermediate file
        proc_next_line();             //read next input line
    }
    // if we reach here, we have seen END
    if(tokens.size() > 1 ) first_exec_address = SYMTAB[tokens[1]]; //save operand in first execution address
    else first_exec_address = start_address ; //save starting address in first execution address

    // output the last line to the intermediate file
    fout << hex << uppercase << LOCCTR;
    fout << line << endl;

    //save (LOCCTR - starting address) as program length
    program_length = int_to_hex(LOCCTR - stoll(start_address, nullptr, 16));

    // finally we close the files
    fin.close();
    fout.close();
}

// ██████╗░░█████╗░░██████╗░██████╗  ██████╗░
// ██╔══██╗██╔══██╗██╔════╝██╔════╝  ╚════██╗
// ██████╔╝███████║╚█████╗░╚█████╗░  ░░███╔═╝
// ██╔═══╝░██╔══██║░╚═══██╗░╚═══██╗  ██╔══╝░░
// ██║░░░░░██║░░██║██████╔╝██████╔╝  ███████╗
// ╚═╝░░░░░╚═╝░░╚═╝╚═════╝░╚═════╝░  ╚══════╝
void pass2() {
    pass_no++;
    line_num = 0;
    // first open the intermediate and object file
    fin.open(intermediate_file_name);
    if (!fin.is_open()) {
        cout << "Error opening intermediary file" << endl;
        exit(0);
    }
    fout.open(object_file_name);
    if (!fout.is_open()) {
        cout << "Error creating object file" << endl;
        exit(0);
    }

    //read first input line (from intermediate file)
    do {
        proc_next_line();
    } while (is_comment );  // skip comments 
    
    // write the header
    line = "H" + fmt_string(program_name, 6, 1, ' ') +
        fmt_string(start_address, 6, 0, '0') +
        fmt_string(program_length, 6, 0, '0');
    fout << line << endl;

    if (tokens[0] == "START") {
        //read next input line
        proc_next_line();
    }

    //initialize first Text record
    bool first_inst = true;        //flag to indicate first instruction in Text record
    string record_starting_address;
    string object_code_string = "";

    //while OPCODE != 'END'
    while (tokens[0] != "END") {
        if (!is_comment) {
            string opcode = tokens[0];

            //search OPTAB for OPCODE
            //if OPCODE is found
            if (OPTAB.find(opcode) != OPTAB.end()) {
                string operand_address;

                //if there is a symbol in operand field
                if ((int) tokens.size() == 2) {
                    string operand = tokens[1];

                    // ,X denotes indexed addressing, we check for that
                    bool is_indexed = false;
                    if(operand.length() > 2 && operand.substr(operand.length() - 2, 2) == ",X") {
                        operand = operand.substr(0, operand.length() - 2);
                        is_indexed = true;
                    }

                    //search SYMTAB for OPERAND
                    //if OPERAND is found
                    if (SYMTAB.find(operand) != SYMTAB.end()) {
                        //store symbol value as operand address
                        operand_address = SYMTAB[operand];
                        operand_address = fmt_string(operand_address, 4, 0, '0');
                        if (is_indexed)
                            operand_address[0] += 8;
                    }
                    //if OPERAND is not found
                    else {
                        error_message = "UNDEFINED SYMBOL";
                        handle_error(2, error_message, line_num, operand);
                    }
                }
                //if there is no symbol in operand field
                else {
                    //store 0 as operand address
                    operand_address = "0";
                    operand_address = fmt_string(operand_address, 4, 0, '0');
                }

                //assemble the object code instruction
                string object_code_instruction = OPTAB[opcode] + operand_address;

                //if object code can fit into the current Text record
                if (object_code_string.length() + object_code_instruction.length() <= 60) {
                    //add object to Text record
                    object_code_string += object_code_instruction;

                    //if record has not started yet, set starting
                    //address to LOCCTR of current instruction
                    if (first_inst) {
                        record_starting_address = int_to_hex(LOCCTR);
                        first_inst = false;
                    }
                }
                //if object code will not fit into the current Text record
                else {
                    string length_of_current_record = int_to_hex(object_code_string.length() / 2);
                    string Text_record = "T" + fmt_string(record_starting_address, 6, 0, '0') +
                        fmt_string(length_of_current_record, 2, 0, '0') +
                        object_code_string;

                    //write Text record to object program
                    fout << Text_record << endl;

                    //initialize new Text record
                    record_starting_address = int_to_hex(LOCCTR);
                    object_code_string = object_code_instruction;
                }
            }
            //if OPCODE = 'BYTE' or 'WORD'
            else if (opcode == "BYTE" || opcode == "WORD") {
                string object_code_of_constant = "";

                //if OPCODE = 'BYTE'
                if (opcode == "BYTE") {
                    string constant = tokens[1];
                    char type = constant[0];
                    constant = constant.substr(2, constant.length() - 3);

                    //if it is a character constant
                    if (type == 'C') {
                        // convert the character array into hexadecimal string
                        for (char& u: constant) {
                            long long num = u;
                            string hexadecimal_value = int_to_hex(num);
                            object_code_of_constant += fmt_string(hexadecimal_value, 2, 0, '0');
                        }
                    }
                    //if it is a hexadecimal constant
                    else if (type == 'X') {
                        object_code_of_constant = constant;
                    }
                }
                //if OPCODE = 'WORD'
                else if (opcode == "WORD") {
                    long long num = stoll(tokens[1]);

                    //convert constant to object code
                    object_code_of_constant = int_to_hex(num);
                    object_code_of_constant = fmt_string(object_code_of_constant, 6, 0, '0');
                }

                //if object code can fit into the current Text record
                if (object_code_string.length() + object_code_of_constant.length() <= 60) {
                    //add object to Text record
                    object_code_string += object_code_of_constant;

                    //if record has not started yet, set starting
                    //address to LOCCTR of current instruction
                    if (first_inst) {
                        record_starting_address = int_to_hex(LOCCTR);
                        first_inst = false;
                    }
                }
                //if object code will not fit into the current Text record
                else {
                    string length_of_current_record = int_to_hex(object_code_string.length() / 2);
                    string Text_record = "T" + fmt_string(record_starting_address, 6, 0, '0') +
                        fmt_string(length_of_current_record, 2, 0, '0') +
                        object_code_string;

                    //write Text record to object program
                    fout << Text_record << endl;

                    //initialize new Text record
                    record_starting_address = int_to_hex(LOCCTR);
                    object_code_string = object_code_of_constant;
                }
            }
            //if OPCODE = 'RESB' or 'RESW'
            else {
                //if there is a non-empty Text record
                if (object_code_string != "") {
                    string length_of_current_record = int_to_hex(object_code_string.length() / 2);
                    string Text_record = "T" + fmt_string(record_starting_address, 6, 0, '0') +
                        fmt_string(length_of_current_record, 2, 0, '0') +
                        object_code_string;

                    //write Text record to object program
                    fout << Text_record << endl;

                    //initialize new empty Text record
                    //set Text record as not started
                    object_code_string = "";
                    first_inst = true;
                }
            }
        }

        //read next input line
        proc_next_line();
    }

    //write last Text record to object program
    if (object_code_string != "") {
        string length_of_current_record = int_to_hex(object_code_string.length() / 2);
        string last_Text_record = "T" + fmt_string(record_starting_address, 6, 0, '0') +
            fmt_string(length_of_current_record, 2, 0, '0') +
            object_code_string;
        fout << last_Text_record << endl;
    }

    //write End record to object program
    string End_record = "E" + fmt_string(first_exec_address, 6, 0, '0');
    fout << End_record << endl;

}

// ███╗░░░███╗░█████╗░██╗███╗░░██╗
// ████╗░████║██╔══██╗██║████╗░██║
// ██╔████╔██║███████║██║██╔██╗██║
// ██║╚██╔╝██║██╔══██║██║██║╚████║
// ██║░╚═╝░██║██║░░██║██║██║░╚███║
// ╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝╚═╝░░╚══╝

int main(int argc, char * argv[]) {

    // first get the file name
    if (argc != 2) {
        cout << "Usage: ./a.out <filename>" << endl;
        return 0;
    }
    input_file_name = argv[1];
    // remove the extension
    string fn = input_file_name;
    auto x = fn.find_first_of('.');
    fn = fn.substr(0, x);

    // get corresponding file names
    intermediate_file_name = fn + "_intermediate.txt";
    object_file_name = fn + "_object.txt";

    setup_optab();  // fill the optable

    pass1();

    print_symtab(); // print the symbol table

    pass2();

    return 0;
}