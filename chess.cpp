#include <iostream>
using namespace std;


//initializing global variables
int old_x;   //x-position of piece before moving
int old_y;  //y-position of  piece before moving 
string old_name;   // name of the old piece which is present at the position where new piece is moving to 
bool first = false;   // store if it is the first move of a piece
bool last_pawn_move; // store boolean value of whether the last move was made by a pawn or not
int pawn_x;  // store the x-position of the last pawn move
int pawn_y;  // store the y-position of the last pawn move
string capture_pieces[32] ;  //array which store the pieces which have been captured
int i = 0;  //points to the index where next captured piece will be stored in the array
string attack_name; 
string abc_attack;
int attack_x = -1;
int attack_y = -1;

class piece{   //creating a class piece - each piece has a x-coordinate, y-coordinate, the first move , and a name
    public :
    int x_pos;
    int y_pos;
    bool first_move = true; // store if piece has already moved or not
    string name; // store the name of the piece
    int initial_pos(int x,int y,string label,string my_board[8][8]){   //class method that sets initial positions of the piece according to the parameters passed to it 
        x_pos = x;
        y_pos = y;
        name = label;
        my_board[y][x] = name;
        return 0;
    }
    void update_pos(int x,int y,string my_board[8][8]){   //class method that changes initial positions of the piece according to the parameters passed to it 
        my_board[y_pos][x_pos] = " E "; //the old position - that the piece now leaves vacant - displays  'E'-empty. 
        old_x= x_pos;    //global variable used as temporary variables to update values 
        old_y = y_pos;
        x_pos = x;
        y_pos = y;
        my_board[y_pos][x_pos] = name;   //update name of the piece on the updated poisition on the board 
    }
    
    int get_capture(string my_board[8][8]){  //store the name of the piece captured in the captured piece array
        x_pos = -1000;
        y_pos = -1000;
        capture_pieces[i] = name;
        i++;
        return 0;
    }
 

};


//a class for each new type of piece is initialized which inherits publically from the base class - piece 

class king : public piece{
    public:

    bool is_check;  //store if the king is under check or not 
    bool valid_move(int x,int y,string my_board[8][8]){ //checks if the move entered is valid and returns true/false accordingly
        int dx = x_pos - x;
        int dy = y_pos - y;
        if (x < 0 || x > 7 || y < 0 || y > 7){
            return false;
        }
        //check if change in x and y is not more than 1 
        else if ((dx == 1 || dx == -1 || dx == 0) && ( dy == 0 || dy == -1 || dy == 1) && my_board[y][x][0] != name[0]){
            return true;
        }
        //check for castling conditions
        else if(first_move == true && (dx == 2 || dx == -2 ) && dy == 0 && my_board[y][x][0] != name[0] && is_check == false) {
            return true;
        }
        else {
            return false;
        }
    }
};

class queen : public piece{
    public:
    bool valid_move(int x,int y,string my_board[8][8]){  //checks if the move entered is valid and returns true/false accordingly
        int dx = x - x_pos;
        int dy = y - y_pos;
        if (x > 7 || y > 7 || x < 0 || y < 0 ){
            return false;
        }
        if (((y == y_pos && x != x_pos)|| (y != y_pos && x == x_pos)) && my_board[y][x][0] != name[0]){
            //check if queen is moving horizontally/vertically correctly
            //by checking that only one coordinate is changing and other is same
            // and there is no piece present between the queen current position and updated position 
            if (x == x_pos){
            if (y > y_pos){
                for (int c = y_pos + 1; c< y ;c++){
                    if (my_board[c][x] != " E "){
                        return false;
                    }
 
                }
            }
            else{
                for (int c = y_pos - 1; c > y ;c--){
                    if (my_board[c][x] != " E "){
                        return false;
                    }
                }
 
            }
            }
            else if (y == y_pos){
                if (x > x_pos){
                    for (int c = x_pos + 1; c< x ;c++){
                        if (my_board[y][c] != " E "){
                            return false;
                        }
    
                    }
                }
                else{
                    for (int c = x_pos - 1; c > x ;c--){
                        if (my_board[y][c] != " E "){
                            return false;
                        }
                    }
    
                }
    
            }
            return true;
            
        }
        else if((dy == dx || dy == -dx) && dy != 0 && my_board[y][x][0] != name[0]) {
             //check if queen is moving diagonally correctly
             //by checking that change in x is equal to change in y 
             // and there is no piece present between the queen current position and updated position 
            int r = y_pos;
            if (x > x_pos){
                for (int c = x_pos+1;c < x;c++ ){
                    if (y < y_pos){
                        r--;
                    }
                    else {
                        r++;
                    }
            
                    if (my_board[r][c] != " E "){
                        return false;
                    }
            }
            }
            else {
                for (int c = x_pos-1;c > x;c-- ){
                    if (y < y_pos){
                        r--;
                    }
                    else {
                        r++;
                    }
                
                    if (my_board[r][c] != " E "){
                        return false;
                    }
                }
            }
            return true;
        }
        else {
            return false;
        }
    }
};

class knight : public piece{
    public:
        bool valid_move(int x,int y,string my_board[8][8]){ //checking knight is moving in L shaped
        int dx = x - x_pos;
        int dy = y - y_pos;
        if (x > 7 || y > 7 || x < 0 || y < 0){
            return false;
        }
        if ((((dx == 2 || dx == -2) && (dy == 1 || dy == -1))|| (dx == 1 || dx == -1) && (dy == 2 || dy == -2)) && my_board[y][x][0] != name[0] ){
            return true;
        }
        else {
            return false;
        }
    }
};

class rook : public piece{
    public:
    bool valid_move(int x,int y,string my_board[8][8]){
        //checks if the move entered is valid and returns true/false accordingly
        //check if rook is moving horizontally/vertically correctly
        //by checking that only one coordinate is changing and other is same
        // and there is no piece present between the rook current position and updated position 
        int dx = x - x_pos;
        int dy = y - y_pos;
        if (x > 7 || y > 7 || x <0 || y < 0 ){
            return false;
        }
        if (((y == y_pos && x != x_pos)|| (y != y_pos && x == x_pos))&& my_board[y][x][0] != name[0] ){
            if (x == x_pos){
            if (y > y_pos){
                for (int c = y_pos + 1; c< y ;c++){
                    if (my_board[c][x] != " E "){
                        return false;
                    }
 
                }
            }
            else{
                for (int c = y_pos - 1; c > y ;c--){
                    if (my_board[c][x] != " E "){
                        return false;
                    }
                }
 
            }
        }
        else if (y == y_pos){
            if (x > x_pos){
                for (int c = x_pos + 1; c< x ;c++){
                    if (my_board[y][c] != " E "){
                        return false;
                    }
 
                }
            }
            else{
                for (int c = x_pos - 1; c > x ;c--){
                    if (my_board[y][c] != " E "){
                        return false;
                    }
                }
 
            }
 
        }
            return true;
        }
        else {
            return false;
        }
    }
};

class bishop : public piece{
    public:
    bool valid_move(int x,int y,string my_board[8][8]){
        int dx = x - x_pos;
        int dy = y - y_pos;
        if (x > 7 || y > 7 || x < 0 || y < 0 ){
            return false;
        }
        if((dy == dx || dy == -dx) && dy != 0 && my_board[y][x][0] != name[0]) {
            int r = y_pos;
            if (x > x_pos){
            for (int c = x_pos+1;c < x;c++ ){
                if (y < y_pos){
                    r--;
                }
                else {
                    r++;
                }
           
                if (my_board[r][c] != " E "){
                    return false;
                }
        }
        }
        else {
            for (int c = x_pos-1;c > x;c-- ){
                if (y < y_pos){
                    r--;
                }
                else {
                    r++;
                }
               
                if (my_board[r][c] != " E "){
                    return false;
                }
            }
        }
       
            return true;
        }
        else {
            return false;
        }
    }
};

class pawn : public piece{
    public:
    bool pawn_colour_check(int y){ //make sure pawns only move forward not backward
        if ((y < y_pos && name[0] == 'W')||(y > y_pos && name[0] != 'W')){
            return false;
        }
        return true;
    }

    bool valid_move(int x,int y,string my_board[8][8]){
        int dx = x - x_pos;
        int dy = y - y_pos;

        if (x > 7 || y > 7 || x <0 || y < 0 ){
            return false;
        }
        if (dx == 0 && (dy == 1 || dy == -1) && my_board[y][x]== " E " && pawn_colour_check(y)){ //check pawn only dispaces 1 place verticallty
            return (true) ;
        }
        else if (first_move == true && (dy == 2 || dy == -2)&& dx == 0 && my_board[y][x] == " E " && pawn_colour_check(y)){ //check pawn move two places vertically only if it is its first move
              return (true);
        }
        else if ((dx == 1 || dx == -1) && (dy == 1|| dy == -1)  &&  my_board[y][x][0]!= name[0] &&  my_board[y][x] != " E " && pawn_colour_check(y)){  //check that pawn only move diagonally if it can capture
            return (true );
        }
        else if((dx == 1 || dx == -1) && (dy == 1|| dy == -1)  &&  my_board[y][x][0]!= name[0] &&  my_board[y][x] == " E " && pawn_colour_check(y) && last_pawn_move == true && abs(pawn_x-x) == 0 && abs(pawn_y-y) == 1 ){ //check if pawn can en-passaunt
            capture_pieces[i] = my_board[pawn_y][pawn_x];
            i++;
            my_board[pawn_y][pawn_x] = " E ";
            
            return true;
        }
 
        else {
            return false;
        }
    }
};

class board{
    public:
    //on board each player has 8 pawns, a king, a queen,  2 knights, 2 rooks, 2 bishops 
    string my_board[8][8];
    pawn w_p_1;
    pawn w_p_2;
    pawn w_p_3;
    pawn w_p_4;
    pawn w_p_5;
    pawn w_p_6;
    pawn w_p_7;
    pawn w_p_8;
    pawn b_p_1;
    pawn b_p_2;
    pawn b_p_3;
    pawn b_p_4;
    pawn b_p_5;
    pawn b_p_6;
    pawn b_p_7;
    pawn b_p_8;
    king b_k;
    king w_k;
    knight b_n_1;
    knight b_n_2;
    knight w_n_1;
    knight w_n_2;
    queen b_q;
    queen w_q;
    bishop w_b_b;
    bishop w_b_w;
    bishop b_b_b;
    bishop b_b_w;
    rook b_r_1;
    rook b_r_2;
    rook w_r_1;
    rook w_r_2;

   
   bool is_capture_piece(string name){ //function to check if the piece has already been captured
    for (int c = i; c >= 0;c--){
        if (name == capture_pieces[c]){
            cout << "Piece is already captured\n";
            return false;
        }

    }
    return true;
   }

    bool temp_update_position(int x,int y,string name){  // updating position of pieces according to the moves inputted (temporarily) to check whether after the move king will still be in check or not
        old_name = my_board[y][x];
        if (name == "WR1"){
            w_r_1.update_pos(x,y,my_board);
            
        }
        else if (name == "WR2"){
            w_r_2.update_pos(x,y,my_board);
        }
        else if (name == "BR2"){
            b_r_2.update_pos(x,y,my_board);
        }
        else if (name == "BR1"){
            b_r_1.update_pos(x,y,my_board);
        }
        else if (name == "WN2"){
            w_n_2.update_pos(x,y,my_board);
        }
        else if (name == "WN1"){
            w_n_1.update_pos(x,y,my_board);
        }
        else if (name == "BN2"){
            b_n_2.update_pos(x,y,my_board);
        }
        else if (name == "BN1"){
            b_n_1.update_pos(x,y,my_board);
        }
        else if (name == "WQ_"){
            w_q.update_pos(x,y,my_board);
        }
        else if (name == "BQ_"){
            b_q.update_pos(x,y,my_board);
        }
        else if (name == "WK_"){
            w_k.update_pos(x,y,my_board);
        }
        else if (name == "BK_"){
            b_k.update_pos(x,y,my_board);
        }
        else if (name == "WBw"){
            w_b_w.update_pos(x,y,my_board);
        }
        else if (name == "WBb"){
            w_b_b.update_pos(x,y,my_board);
        }
        else if (name == "BBb"){
            b_b_b.update_pos(x,y,my_board);
        }
        else if (name == "BBw"){
            b_b_w.update_pos(x,y,my_board);
        }
        else if (name == "WP1"){
            w_p_1.update_pos(x,y,my_board);
        }
        else if (name == "WP2"){
            w_p_2.update_pos(x,y,my_board);
        }
        else if (name == "WP3"){
            w_p_3.update_pos(x,y,my_board);
        }
        else if (name == "WP4"){
            w_p_4.update_pos(x,y,my_board);
        }
        else if (name == "WP5"){
            w_p_5.update_pos(x,y,my_board);
            
        }
        else if (name == "WP6"){
            w_p_6.update_pos(x,y,my_board);

        }
        else if (name == "WP7"){
            w_p_7.update_pos(x,y,my_board);
             }
        else if (name == "WP8"){
            w_p_8.update_pos(x,y,my_board);
            }
        else if (name == "BP1"){
            b_p_1.update_pos(x,y,my_board);
            }
        else if (name == "BP2"){
            b_p_2.update_pos(x,y,my_board);
            }
        else if (name == "BP3"){
            b_p_3.update_pos(x,y,my_board);
            }
        else if (name == "BP4"){
            b_p_4.update_pos(x,y,my_board);
            }
        else if (name == "BP5"){
            b_p_5.update_pos(x,y,my_board);
            }
        else if (name == "BP6"){
            b_p_6.update_pos(x,y,my_board);
            }
        else if (name == "BP7"){
            b_p_7.update_pos(x,y,my_board);
            }
        else if (name == "BP8"){
            b_p_8.update_pos(x,y,my_board);
            }
    
        if (name[0] == 'W'){
                if (is_king_check("Black") == 0){
                    update_position(old_x,old_y,name);
                    update_position(x,y,old_name);
                    return false;
                }
        }
        else if (name[0] == 'B'){
             if (is_king_check("White") == 0){
                    update_position(old_x,old_y,name);
                    update_position(x,y,old_name);
                    return false;
                }
        }
        update_position(old_x,old_y,name);
        update_position(x,y,old_name);

        return true;
    }

    bool is_king_check(string name){  //function to check if the king is in check
        if (name[0] == 'B'){
            int k_x_pos = w_k.x_pos;
            int k_y_pos = w_k.y_pos;
            if (b_p_1.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_p_1.name){
                return false;
            }
            else if (b_p_2.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_p_2.name){
                return false;
            }
            else if (b_p_3.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_p_3.name){
                return false;
            }
            else if (b_p_4.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_p_4.name){
                return false;
            }
            else if (b_p_5.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_p_5.name){
                return false;
            }
            else if (b_p_6.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_p_6.name){
                return false;
            }
            else if (b_p_7.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_p_7.name){
                return false;
            }
            else if (b_p_8.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_p_8.name){
                return false;
            }
            else if (b_r_2.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_r_2.name){
                return false;
            }
            else if (b_r_1.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_r_1.name){
                return false;
            }
            else if (b_n_2.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_n_2.name){
                return false;
            }
            else if (b_n_1.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_n_1.name){
                return false;
            }
            else if (b_b_b.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_b_b.name){
                return false;
            }
            else if (b_b_w.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_b_w.name){
                return false;
            }
            else if (b_k.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_k.name){
                return false;
            }
            else if (b_q.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != b_q.name){
                return false;
            }

        }
        else{
            int k_x_pos = b_k.x_pos;
            int k_y_pos = b_k.y_pos;
            if (w_p_1.valid_move(k_x_pos,k_y_pos,my_board) == true && abc_attack != w_p_1.name){
                return false;
            }
            else if (w_p_2.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_p_2.name){
                return false;
            }
            else if (w_p_3.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_p_3.name){
                return false;
            }
            else if (w_p_4.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_p_4.name){
                return false;
            }
            else if (w_p_5.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_p_5.name){
                return false;
            }
            else if (w_p_6.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_p_6.name){
                return false;
            }
            else if (w_p_7.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_p_7.name){
                return false;
            }
            else if (w_p_8.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_p_8.name){
                return false;
            }
            else if (w_r_2.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_r_2.name){
                return false;
            }
            else if (w_r_1.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_r_1.name){
                return false;
            }
            else if (w_n_2.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_n_2.name){
                return false;
            }
            else if (w_n_1.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_n_1.name){
                return false;
            }
            else if (w_b_b.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_b_b.name){
                return false;
            }
            else if (w_b_w.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_b_b.name){
                return false;
            }
            else if (w_k.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_k.name){
                return false;
            }
            else if (w_q.valid_move(k_x_pos,k_y_pos,my_board) == true  && abc_attack != w_q.name){
                return false;
            }
            
        }
        
        return true;
    }

    bool can_piece_block(string name){ //function that check whether check can be blocked by moving any other piece
        if (name[1] != 'N' || name[1] != 'P'){
            for (int c = 0;c<= 7;c++){
                for(int r = 0;r <= 7;r++){
                    string board_piece = my_board[c][r];
                    if (board_piece[0] != name[0]){
                    for (int i = 0;i <= 7;i++){
                        for (int j = 0;j <= 7;j++){
                            if (piece_valid_move(j,i,board_piece) == true){
                                if ( temp_update_position(j,i,board_piece) != false){
                                    cout << board_piece << " Can block by moving to " << char(j + 65) << i + 1 << endl; 
                                    return true;
                                }

                            }
                            

                        }
                    }
                    }
                }
            }
        }
        else{
            return false;
        }
        return false;
    }
   
    bool can_king_move(string name){ // function to check if there is any safe square for the king to avoid check
        
        if (name[0] == 'W'){
            int x = b_k.x_pos;
            int y = b_k.y_pos;
            int a = x ;
            int c = x + 1;
            int d = x - 1;
            int e = y;
            int g = y+1;
            int h = y -1;
        
            if ((g < 8) ){
                if (temp_update_position(a,g,"BK_") == true && b_k.valid_move(a,g,my_board) == true){
                    cout << "King can move to " << char(a + 65) << g + 1 << endl;
                    return true;
                }
            }
            
            if ((h >=  0)){
                 if (temp_update_position(a,h,"BK_") == true && b_k.valid_move(a,h,my_board) == true){
                    cout << "King can move to " << char(a + 65) << h + 1 << endl;
                    return true;
                }
            }
          
            if ((c < 8 )){
                 if (temp_update_position(c,e,"BK_") == true && b_k.valid_move(c,e,my_board) == true){
                    cout << "King can move to " << char(c + 65) << e + 1 << endl;
                    return true;
                }
            }
            
            if ((d >= 0)){
                 if (temp_update_position(d,e,"BK_") == true && b_k.valid_move(d,e,my_board) == true){
                    cout << "King can move to " << char(d + 65) << e + 1 << endl;
                    return true;
                }
            }
           
            if (( c < 8 && h >= 0)){
                 if (temp_update_position(c,h,"BK_") == true && b_k.valid_move(c,h,my_board) == true){
                    cout << "King can move to " << char(c + 65) << h + 1 << endl;
                    return true;
                }
            }
            if (( d >= 0 && h >= 0)){
                 if (temp_update_position(d,h,"BK_") == true && b_k.valid_move(d,h,my_board) == true){
                    cout << "King can move to " << char(d + 65) << h + 1 << endl;
                    return true;
                }
            }
            if (( d >= 0 && g < 8)){
                 if (temp_update_position(d,g,"BK_") == true && b_k.valid_move(d,g,my_board) == true){
                    cout << "King can move to " << char(d + 65) << g + 1 << endl;
                    return true;
                }
            }
            if (( c <8 && g < 8)){
                 if (temp_update_position(c,g,"BK_") == true && b_k.valid_move(c,g,my_board) == true){
                    cout << "King can move to " << char(c + 65) << g + 1 << endl;
                    return true;
                }
            }
            
        }
        else {
            int x = w_k.x_pos;
            int y = w_k.y_pos;
            int a = x ;
            int c = x + 1;
            int d = x - 1;
            int e = y;
            int g = y+1;
            int h = y -1;
         
            if ((g < 8)){
                if (temp_update_position(a,g,"WK_") == true && w_k.valid_move(a,g,my_board) == true){
                    cout << "King can move to " << char(a + 65) << g + 1 << endl;
                    return true;
                }
            }
            
            if ((h >=  0)){
                 if (temp_update_position(a,h,"WK_") == true && w_k.valid_move(a,h,my_board) == true){
                    cout << "King can move to " << char(a + 65) << h + 1 << endl;
                    return true;
                }
            }
          
            if ((c < 8 )){
                 if (temp_update_position(c,e,"WK_") == true && w_k.valid_move(c,e,my_board) == true){
                    cout << "King can move to " << char(c + 65) << e + 1 << endl;
                    return true;
                }
            }
            
            if ((d >= 0)){
                 if (temp_update_position(d,e,"WK_") == true && w_k.valid_move(d,e,my_board) == true){
                    cout << "King can move to " << char(d + 65) << e + 1 << endl;
                    return true;
                }
            }
          
            if (( c < 8 && h >= 0)){
                 if (temp_update_position(c,h,"WK_") == true && w_k.valid_move(c,h,my_board) == true){
                    cout << "King can move to " << char(c + 65) << h + 1 << endl;
                    return true;
                }
            }
            if (( d >= 0 && h >= 0)){
                 if (temp_update_position(d,h,"WK_") == true && w_k.valid_move(d,h,my_board) == true){
                    cout << "King can move to " << char(d + 65) << h + 1 << endl;
                    return true;
                }
            }
            if (( d >= 0 && g < 8)){
                 if (temp_update_position(d,g,"WK_") == true && w_k.valid_move(d,g,my_board) == true){
                    cout << "King can move to " << char(d + 65) << g + 1 << endl;
                    return true;
                }
            }
            if (( c <8 && g < 8)){
                 if (temp_update_position(c,g,"WK_") == true && w_k.valid_move(c,g,my_board) == true){
                    cout << "King can move to " << char(c + 65) << g + 1 << endl;
                    return true;
                }
            }

        }
        return false;
    }

    bool can_piece_capture(int k_x_pos,int k_y_pos,string name){ //function that check and displays if the attacking piece can be captured 
        if (name[0] == 'W'){
            
            if (b_p_1.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Pawn 1 can capture\n";
                return true;
            }
            else if (b_p_2.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Pawn 2 can capture\n";
                return true;
            }
            else if (b_p_3.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Pawn 3 can capture\n";
                return true;
            }
            else if (b_p_4.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Pawn 4 can capture\n";
                return true;
            }
            else if (b_p_5.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Pawn 5 can capture\n";
                return true;
            }
            else if (b_p_6.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Pawn 6 can capture\n";
                return true;
            }
            else if (b_p_7.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Pawn 7 can capture\n";
                return true;
            }
            else if (b_p_8.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Pawn 8 can capture\n";
                return true;
            }
            else if (b_r_2.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Rook 2 can capture\n";
                return true;
            }
            else if (b_r_1.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Rook 1 can capture\n";
                return true;
            }
            else if (b_n_2.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Knight 2 can capture\n";
                return true;
            }
            else if (b_n_1.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "KNight 1 can capture\n";
                return true;
            }
            else if (b_b_b.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Black Bishop can capture\n";
                return true;
            }
            else if (b_b_w.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "White Bishop can capture\n";
                return true;
            }
            else if (b_k.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << " King can capture\n";
                return true;
            }
            else if (b_q.valid_move(k_x_pos,k_y_pos,my_board) == true){
                 cout << "Queen can capture\n";
                return true;
            }

        }
        else{

            if (w_p_1.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Pawn 1 can Capture\n";
                return true;
            }
            else if (w_p_2.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Pawn 2 can Capture\n";
                return true;
            }
            else if (w_p_3.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Pawn 3 can Capture\n";
                return true;
            }
            else if (w_p_4.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Pawn 4 can Capture\n";
                return true;
            }
            else if (w_p_5.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Pawn 5 can Capture\n";
                return true;
            }
            else if (w_p_6.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Pawn 6 can Capture\n";
                return true;
            }
            else if (w_p_7.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Pawn 7 can Capture\n";
                return true;
            }
            else if (w_p_8.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Pawn 8 can Capture\n";
                return true;
            }
            else if (w_r_2.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Rook 2 can Capture\n";
                return true;
            }
            else if (w_r_1.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Rook 1 can Capture\n";
                return true;
            }
            else if (w_n_2.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Night 2 can Capture\n";
                return true;
            }
            else if (w_n_1.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Night 1 can Capture\n";
                return true;
            }
            else if (w_b_b.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Black Bishop can Capture\n";
                return true;
            }
            else if (w_b_w.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "White Bishop can Capture\n";
                return true;
            }
            else if (w_k.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "King can Capture\n";
                return true;
            }
            else if (w_q.valid_move(k_x_pos,k_y_pos,my_board) == true){
                cout << "Queen can Capture\n";
                return true;
            }
        }
        return false;
        
    }

    bool is_checkmate(int x,int y,string name){  //function to find if opponent is checkmated
        bool piece_capture = can_piece_capture(x,y,name);
        bool possible_move = can_king_move(name);
        bool piece_move = can_piece_block(name);
        cout <<  piece_move << " Piece can block\n";
        return !(piece_capture || possible_move || piece_move);
    }
    
    void capture(int x,int y){  //function to capture a piece
        if (my_board[y][x] == "WR1"){
            w_r_1.get_capture(my_board);
            
        }
        else if (my_board[y][x] == "WR2"){
            w_r_2.get_capture(my_board);
        }
        else if (my_board[y][x] == "BR2"){
            b_r_2.get_capture(my_board);
        }
        else if (my_board[y][x] == "BR1"){
            b_r_1.get_capture(my_board);
        }
        else if (my_board[y][x] == "WN2"){
            w_n_2.get_capture(my_board);
        }
        else if (my_board[y][x] == "WN1"){
            w_n_1.get_capture(my_board);
        }
        else if (my_board[y][x] == "BN2"){
            b_n_2.get_capture(my_board);
        }
        else if (my_board[y][x] == "BN1"){
            b_n_1.get_capture(my_board);
        }
        else if (my_board[y][x] == "WQ_"){
            w_q.get_capture(my_board);
        }
        else if (my_board[y][x] == "BQ_"){
            b_q.get_capture(my_board);
        }
        else if (my_board[y][x] == "WK_"){
            w_k.get_capture(my_board);
        }
        else if (my_board[y][x] == "BK_"){
            b_k.get_capture(my_board);
        }
        else if (my_board[y][x] == "WBw"){
            w_b_w.get_capture(my_board);
        }
        else if (my_board[y][x] == "WBb"){
            w_b_b.get_capture(my_board);
        }
        else if (my_board[y][x] == "BBb"){
            b_b_b.get_capture(my_board);
        }
        else if (my_board[y][x] == "BBw"){
            b_b_w.get_capture(my_board);
        }
        else if (my_board[y][x] == "WP1"){
            w_p_1.get_capture(my_board);
        }
        else if (my_board[y][x] == "WP2"){
            w_p_2.get_capture(my_board);
        }
        else if (my_board[y][x] == "WP3"){
            w_p_3.get_capture(my_board);
        }
        else if (my_board[y][x] == "WP4"){
            w_p_4.get_capture(my_board);
        }
        else if (my_board[y][x] == "WP5"){
            w_p_5.get_capture(my_board);
        }
        else if (my_board[y][x] == "WP6"){
            w_p_6.get_capture(my_board);
        }
        else if (my_board[y][x] == "WP7"){
            w_p_7.get_capture(my_board);
        }
        else if (my_board[y][x] == "WP8"){
            w_p_8.get_capture(my_board);
        }
        else if (my_board[y][x] == "BP1"){
            b_p_1.get_capture(my_board);
        }
        else if (my_board[y][x] == "BP2"){
            b_p_2.get_capture(my_board);}
        else if (my_board[y][x] == "BP3"){
            b_p_3.get_capture(my_board);}
        else if (my_board[y][x] == "BP4"){
            b_p_4.get_capture(my_board);}
        else if (my_board[y][x] == "BP5"){
            b_p_5.get_capture(my_board);}
        else if (my_board[y][x] == "BP6"){
            b_p_6.get_capture(my_board);}
        else if (my_board[y][x] == "BP7"){
            b_p_7.get_capture(my_board);}
        else if (my_board[y][x] == "BP8"){
            b_p_8.get_capture(my_board);
        }
    }

    int update_position(int x,int y,string name){ //updating position of pieces according to the moves entered 
        if (name == "WR1"){
            w_r_1.update_pos(x,y,my_board);
            
        }
        else if (name == "WR2"){
            w_r_2.update_pos(x,y,my_board);
        }
        else if (name == "BR2"){
            b_r_2.update_pos(x,y,my_board);
        }
        else if (name == "BR1"){
            b_r_1.update_pos(x,y,my_board);
        }
        else if (name == "WN2"){
            w_n_2.update_pos(x,y,my_board);
        }
        else if (name == "WN1"){
            w_n_1.update_pos(x,y,my_board);
        }
        else if (name == "BN2"){
            b_n_2.update_pos(x,y,my_board);
        }
        else if (name == "BN1"){
            b_n_1.update_pos(x,y,my_board);
        }
        else if (name == "WQ_"){
            w_q.update_pos(x,y,my_board);
        }
        else if (name == "BQ_"){
            b_q.update_pos(x,y,my_board);
        }
        else if (name == "WK_"){
            w_k.update_pos(x,y,my_board);
        }
        else if (name == "BK_"){
            b_k.update_pos(x,y,my_board);
        }
        else if (name == "WBw"){
            w_b_w.update_pos(x,y,my_board);
        }
        else if (name == "WBb"){
            w_b_b.update_pos(x,y,my_board);
        }
        else if (name == "BBb"){
            b_b_b.update_pos(x,y,my_board);
        }
        else if (name == "BBw"){
            b_b_w.update_pos(x,y,my_board);
        }
        else if (name == "WP1"){
            w_p_1.update_pos(x,y,my_board);
        }
        else if (name == "WP2"){
            w_p_2.update_pos(x,y,my_board);
        }
        else if (name == "WP3"){
            w_p_3.update_pos(x,y,my_board);
        }
        else if (name == "WP4"){
            w_p_4.update_pos(x,y,my_board);
        }
        else if (name == "WP5"){
            w_p_5.update_pos(x,y,my_board);
        }
        else if (name == "WP6"){
            w_p_6.update_pos(x,y,my_board);
        }
        else if (name == "WP7"){
            w_p_7.update_pos(x,y,my_board);
        }
        else if (name == "WP8"){
            w_p_8.update_pos(x,y,my_board);
        }
        else if (name == "BP1"){
            b_p_1.update_pos(x,y,my_board);
        }
        else if (name == "BP2"){
            b_p_2.update_pos(x,y,my_board);
        }
        else if (name == "BP3"){
            b_p_3.update_pos(x,y,my_board);
        }
        else if (name == "BP4"){
            b_p_4.update_pos(x,y,my_board);
        }
        else if (name == "BP5"){
            b_p_5.update_pos(x,y,my_board);
        }
        else if (name == "BP6"){
            b_p_6.update_pos(x,y,my_board);
        }
        else if (name == "BP7"){
            b_p_7.update_pos(x,y,my_board);
        }
        else if (name == "BP8"){
            b_p_8.update_pos(x,y,my_board);            
        }
        return 0;
    }

    bool piece_valid_move(int x,int y,string name){ //checking if the move inputted is valid or not 
        if (name == "WR1"){
            return w_r_1.valid_move(x,y,my_board);
        }
        else if (name == "WR2"){
            return w_r_2.valid_move(x,y,my_board);
        }
        else if (name == "BR2"){
            return b_r_2.valid_move(x,y,my_board);
        }
        else if (name == "BR1"){
            return b_r_1.valid_move(x,y,my_board);
        }
        else if (name == "WN2"){
            return w_n_2.valid_move(x,y,my_board);
        }
        else if (name == "WN1"){
            return w_n_1.valid_move(x,y,my_board);
        }
        else if (name == "BN2"){
            return b_n_2.valid_move(x,y,my_board);
        }
        else if (name == "BN1"){
            return b_n_1.valid_move(x,y,my_board);
        }
        else if (name == "WQ_"){
            return w_q.valid_move(x,y,my_board);
        }
        else if (name == "BQ_"){
            return b_q.valid_move(x,y,my_board);
        }
        else if (name == "WK_"){
            return w_k.valid_move(x,y,my_board);
        }
        else if (name == "BK_"){
            return b_k.valid_move(x,y,my_board);
        }
        else if (name == "WBw"){
            return w_b_w.valid_move(x,y,my_board);
        }
        else if (name == "WBb"){
            return w_b_b.valid_move(x,y,my_board);
        }
        else if (name == "BBb"){
            return b_b_b.valid_move(x,y,my_board);
        }
        else if (name == "BBw"){
            return b_b_w.valid_move(x,y,my_board);
        }
        else if (name == "WP1"){
            return w_p_1.valid_move(x,y,my_board);
        }
        else if (name == "WP2"){
            return w_p_2.valid_move(x,y,my_board);
        }
        else if (name == "WP3"){
            return w_p_3.valid_move(x,y,my_board);
        }
        else if (name == "WP4"){
            return w_p_4.valid_move(x,y,my_board);
        }
        else if (name == "WP5"){
            return w_p_5.valid_move(x,y,my_board);
        }
        else if (name == "WP6"){
            return w_p_6.valid_move(x,y,my_board);
        }
        else if (name == "WP7"){
            return w_p_7.valid_move(x,y,my_board);
        }
        else if (name == "WP8"){
            return w_p_8.valid_move(x,y,my_board);
        }
        else if (name == "BP1"){
            return b_p_1.valid_move(x,y,my_board);
        }
        else if (name == "BP2"){
            return b_p_2.valid_move(x,y,my_board);
        }
        else if (name == "BP3"){
            return b_p_3.valid_move(x,y,my_board);
        }
        else if (name == "BP4"){
            return b_p_4.valid_move(x,y,my_board);
        }
        else if (name == "BP5"){
            return b_p_5.valid_move(x,y,my_board);
        }
        else if (name == "BP6"){
            return b_p_6.valid_move(x,y,my_board);
        }
        else if (name == "BP7"){
            return b_p_7.valid_move(x,y,my_board);
        }
        else if (name == "BP8"){
            return b_p_8.valid_move(x,y,my_board);
        }
        else {
            return false;
        }
      

    }

    bool colour_check(bool num,string name){  //check if players are moving correct pieces Player 1: White Player 2 : Black
        if (num == false && name[0] == 'W'){
            return true;
        }
        else if (num == true && name[0] != 'W'){
            return true;
        }
        else {
            return false;
        }
    }
    
    bool update_first_move(string name){ //update first move of the pieces
        if (name == "WR1"){
            w_r_1.first_move = false;
        }
        else if (name == "WR2"){
            w_r_2.first_move = false;
        }
        else if (name == "BR2"){
            b_r_2.first_move = false;
        }
        else if (name == "BR1"){
            b_r_1.first_move = false;
        }
        else if (name == "WN2"){
            w_n_2.first_move = false;
        }
        else if (name == "WN1"){
            w_n_1.first_move = false;
        }
        else if (name == "BN2"){
            b_n_2.first_move = false;
        }
        else if (name == "BN1"){
            b_n_1.first_move = false;
        }
        else if (name == "WQ_"){
            w_q.first_move = false;
        }
        else if (name == "BQ_"){
            b_q.first_move = false;
        }
        else if (name == "WK_"){
            w_k.first_move = false;
        }
        else if (name == "BK_"){
            b_k.first_move = false;
        }
        else if (name == "WBw"){
            w_b_w.first_move = false;
        }
        else if (name == "WBb"){
            w_b_b.first_move = false;
        }
        else if (name == "BBb"){
            b_b_b.first_move = false;
        }
        else if (name == "BBw"){
            b_b_w.first_move = false;
        }
        else if (name == "WP1"){
            first = w_p_1.first_move;
            w_p_1.first_move = false;
        }
        else if (name == "WP2"){
            first = w_p_2.first_move;
            w_p_2.first_move = false;
        }
        else if (name == "WP3"){
            first = w_p_3.first_move;
            w_p_3.first_move = false;
        }
        else if (name == "WP4"){
            first = w_p_4.first_move;
            w_p_4.first_move = false;
        }
        else if (name == "WP5"){
            first = w_p_5.first_move;
            w_p_5.first_move = false;
        }
        else if (name =="WP6"){
            first = w_p_6.first_move;
            w_p_6.first_move = false;
        }
        else if (name == "WP7"){
            first = w_p_7.first_move;
            w_p_7.first_move = false;
        }
        else if (name == "WP8"){
            first = w_p_8.first_move;
            w_p_8.first_move = false;
        }
        else if (name == "BP1"){
            first = b_p_1.first_move;
            b_p_1.first_move = false;
        }
        else if (name == "BP2"){
            first = b_p_2.first_move;
            b_p_2.first_move = false;
        }
        else if (name == "BP3"){
            first = b_p_3.first_move;
            b_p_3.first_move = false;
        }
        else if (name == "BP4"){
            first = b_p_4.first_move;
            b_p_4.first_move = false;
        }
        else if (name == "BP5"){
            first = b_p_5.first_move;
            b_p_5.first_move = false;
        }
        else if (name == "BP6"){
            first = b_p_6.first_move;
            b_p_6.first_move = false;
        }
        else if (name == "BP7"){
            first = b_p_7.first_move;
            b_p_7.first_move = false;
        }
        else if (name == "BP8"){
            first = b_p_8.first_move;
            b_p_8.first_move = false;            
        }
        return 0;
    }

    int print_board(){ //printing the game board
        cout << "    ";
        for (int c = 0;c <= 7;c++){
            cout << char(c+65) << "     ";
        }
        cout << endl;
        for (int c = 0;c<= 7;c++){
            cout << c+1 << "  ";
            for (int r = 0;r<= 7;r++){
                cout << my_board[c][r] << "   ";
            }
            cout << endl;
        }

        return 0;
    }
 
    int set_board(){ //setting initial positions of all pieces and passing the respective parametres to the intial_pos function
        w_r_1.initial_pos(0,0,"WR1",my_board);
        w_r_2.initial_pos(7,0,"WR2",my_board);
        b_r_1.initial_pos(0,7,"BR1",my_board);
        b_r_2.initial_pos(7,7,"BR2",my_board);
        w_n_1.initial_pos(1,0,"WN1",my_board);
        w_n_2.initial_pos(6,0,"WN2",my_board);
        b_n_2.initial_pos(6,7,"BN2",my_board);
        b_n_1.initial_pos(1,7,"BN1",my_board);
        w_b_w.initial_pos(5,0,"WBw",my_board);
        w_b_b.initial_pos(2,0,"WBb",my_board);
        b_b_w.initial_pos(2,7,"BBw",my_board);
        b_b_b.initial_pos(5,7,"BBb",my_board);
        w_k.initial_pos(4,0,"WK_",my_board);
        b_k.initial_pos(4,7,"BK_",my_board);
        w_q.initial_pos(3,0,"WQ_",my_board);
        b_q.initial_pos(3,7,"BQ_",my_board);
        w_p_1.initial_pos(0,1,"WP1",my_board);
        w_p_2.initial_pos(1,1,"WP2",my_board);
        w_p_3.initial_pos(2,1,"WP3",my_board);
        w_p_4.initial_pos(3,1,"WP4",my_board);
        w_p_5.initial_pos(4,1,"WP5",my_board);
        w_p_6.initial_pos(5,1,"WP6",my_board);
        w_p_7.initial_pos(6,1,"WP7",my_board);
        w_p_8.initial_pos(7,1,"WP8",my_board);
        b_p_1.initial_pos(0,6,"BP1",my_board);
        b_p_2.initial_pos(1,6,"BP2",my_board);
        b_p_3.initial_pos(2,6,"BP3",my_board);
        b_p_4.initial_pos(3,6,"BP4",my_board);
        b_p_5.initial_pos(4,6,"BP5",my_board);
        b_p_6.initial_pos(5,6,"BP6",my_board);
        b_p_7.initial_pos(6,6,"BP7",my_board);
        b_p_8.initial_pos(7,6,"BP8",my_board);
        for (int c = 2;c <= 5;c++){
            for (int r = 0;r<= 7;r++){
                my_board[c][r] = " E ";
            }
        }
        print_board();
        return 0;
    }

    void update_check(string name){  //updating check on the king
        if (name[0] == 'B'){
            w_k.is_check = true;
        }
        else{
            b_k.is_check = true;
        }
    }

    void remove_check(string name){ //removing check from the king
         if (name[0] == 'W'){
            w_k.is_check = false;
        }
        else{
            b_k.is_check = false;
        }
    }

    bool is_last_pawn_move(int x,int y,string name){ //checking if the current move is made by the pawn , and it is the first move of that pawn
          
        if (name[1] == 'P' && first == true){
            
            pawn_x = x;
            pawn_y = y;
            return true;
        }
        else{
            return false;
        }
        
    }

    bool check_empty(int x_from,int x_to,int y){ //function to check if the position to which a piece wants to move is empty or not
        for (int c = x_from;c <= x_to;c++){
            if (my_board[y][c] != " E "){
                return false;
            }
        }
        return true;
    }

    bool castling(int x,int y,string name){ //function for castle
        
        if (name == "WK_"){
            old_x = w_k.x_pos;
            old_y = w_k.y_pos;
            if (abs(old_x-x) == 2){
            if (x > old_x){
                if (check_empty(old_x+1,w_r_2.x_pos-1,y) == true && w_r_2.first_move == true){
                     w_r_2.update_pos(w_r_2.x_pos -2,w_r_2.y_pos,my_board);
                     w_r_2.first_move = false;
                     return true;
            }
                }
               
            else{
                if (check_empty(w_r_1.x_pos+1,old_x-1,y) == true && w_r_1.first_move == true){
                w_r_1.update_pos(w_r_1.x_pos + 3,w_r_2.y_pos,my_board);
                w_r_1.first_move = false;
                return true;
            }
            }
            }
            else{
                return true;
            }
        }
        else if (name == "BK_"){
            old_x = b_k.x_pos;
            old_y = b_k.y_pos;
            if (abs(old_x-x) == 2){
            if (x > old_x){
                if (check_empty(old_x+1,b_r_2.x_pos-1,y) == true && b_r_2.first_move == true){
                b_r_2.update_pos(b_r_2.x_pos -2,b_r_2.y_pos,my_board);
                b_r_2.first_move = false;
                return true;
            }
            }
            else{
                if (check_empty(w_r_1.x_pos+1,old_x-1,y) == true && b_r_1.first_move == true){
                b_r_1.update_pos(b_r_1.x_pos + 3,b_r_1.y_pos,my_board);
                b_r_1.first_move = false;
                return true;
            }}}
            else{
                return true;
            }
        }
        
        return false;
    }

};

//function that displays the player name by checking the bool value passed to it from the startGame function, 
//for example - after a valid move in the startGame, the function changes the value of the bool variable num and the player name is displayed according to that 
void name1(bool num, string arr[])
{
    string name; 
    if(num==true)
    {
        name=arr[1]; 
    }
    if(num==false)
    {
        name=arr[0]; 
    }

   cout<<name; 
}

int startgame(string arr[]){
    string name; //piece name 
    string move; //the position they want to move to - according to the chess board thats displayed 
    board board_1;
    bool num = false;         //initializing a bool to keep track of the players' turn 
    bool valid = false;       //initializing a bool to check the validity of a move inputted by the user 
    bool checkmate = false;   //initializing a bool to check for checkmates after each move 
    int x_pos,y_pos;
    board_1.set_board();
    do{
        cout << "Player ";   //in each loop we display the player name 
        name1(num, arr); 
        cout<< "'s  Turn: \n";
       
        do {
        cout << "Piece Name: ";  //inputting the piece they want to move 
        cin >> name;
         for(int i =0;i<name.length()-1;i++) // for loop for case sensitivity - so that inputs in small letters and capital letters are all valid 
        {
            if(name[i]>=97 && name[i]<=122 ) 
            {
                name[i]=name[i]-32; 
            }
            else{
                name[i]=name[i]; 
            }
        }
        if(name[2]>=65 && name[2]<=90 && name[1] == 'B'){name[2]=name[2]+32; }
        cout << "Move: ";
        cin >> move;  //inputting the position they want to move to 
           for(int i =0;i<move.length();i++)  // for loop for case sensitivity - so that inputs in small letters and capital letters are all valid 
        {
            if(move[i]>=97 && name[i]<=122 )
            {
                move[i]=move[i]-32; 
            }
            else{
                move[i]=move[i]; 
            }
        }
        x_pos = move[0] - 65;   //slicing and storing the x-position of the move
        y_pos = move[1] -49;    //slicing and storing the y-position of the move

        valid = board_1.piece_valid_move(x_pos,y_pos,name) && board_1.colour_check(num,name) && board_1.is_capture_piece(name);
        
       
        if (valid != true){
            cout << "Invalid\n";
        }
        else{
            if (x_pos == attack_x && y_pos == attack_y){
                abc_attack = attack_name;
            }
            else{
                abc_attack = "";
            }
            valid =board_1.temp_update_position(x_pos,y_pos,name);
            if (valid!= true){  //storing whether the piece can move or not depending on whether the king is in check or the piece is pinned to the king
                cout << "Pinned,Cannot Move" << endl;
            }
            else{
                 if (name[1] == 'K'){ // checking if the piece name is king and if it can castle if the king is displaced by 2 places horizontally
            valid = valid && board_1.castling(x_pos,y_pos,name);
        }
            }
           
        }
        }
        while (valid != true);
    board_1.remove_check(name); //remove check 
    board_1.capture(x_pos,y_pos); //capture the piece at the x,y position
    board_1.update_position(x_pos,y_pos,name); // update position of the piece
    board_1.update_first_move(name); //update first move of the piece
    last_pawn_move =  board_1.is_last_pawn_move(x_pos,y_pos,name);  //storing whether this move was a pawn first move ort not
    if (board_1.is_king_check(name)== 0){   //checking if king is in check
        board_1.update_check(name);
        checkmate = board_1.is_checkmate(x_pos,y_pos,name);
        num= !num;  
        cout << "Player " ;
        name1(num, arr);
        cout << " is under check\n";
        num = !num;
        attack_x = x_pos;
        attack_y = y_pos;
        attack_name = name;
    }
    else{
        attack_x = -1;
        attack_y = -1;
        attack_name = "";
    }
    board_1.print_board();  //printing the updated board 
    num = !num;  //updating the num bool after everyturn 
    }while (checkmate != true);

        
    
    return num;
}

int main(){
cout<<"Enter the name for Player 1 : ";   //inputting player name from the users 
string playerName1; 
cin>>playerName1; 
cout<<"Enter the name for Player 2 : "; 
string playerName2; 
cin>>playerName2; 
string array [2]; 
array[0]=playerName1; 
array[1]=playerName2; 
    string choice;
   do {
       
        name1(!startgame(array),array); //calling the game function 
        cout  << " wins\n";
        cout << "Do you wanna play again? yes/no: ";
        cin >> choice;}
    while (choice == "yes");
}