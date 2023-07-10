#include<iostream>
using namespace std;
bool isPieceFree(char** arr, char piece, int FixedRow, int COL, char turn, int newFixedRow, int NewCOL, bool mateCheck = false);
bool isLegalMove(char** arr, char piece, int FixedRow, int COL, int fieldSide, char turn, char moveType, int newFixedRow = 0, int NewCOL = 0, bool mateCheck = false);
void generate(char* arr[]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) { //fields
            if ((2 <= i) && (i <= 5)) {
                arr[i][j] = '=';
            }
            else if (i == 0 || i == 7) {//not pawns
                if (j == 0 || j == 7) { //ROOK
                    arr[i][j] = (i == 0) ? 'r' : 'R';
                }
                else if (j == 1 || j == 6) { //knight (N)
                    arr[i][j] = (i == 0) ? 'n' : 'N';
                }
                else if (j == 2 || j == 5) { //bishop 
                    arr[i][j] = (i == 0) ? 'b' : 'B';
                }
                else if (j == 3) { //queen
                    arr[i][j] = (i == 0) ? 'q' : 'Q';
                }
                else if (j == 4) { //King (K)
                    arr[i][j] = (i == 0) ? 'k' : 'K';
                }
            }
            else if (i == 1 || i == 6) {// pawns
                arr[i][j] = (i == 1) ? 'p' : 'P';
            }
        }
    }
}

void ShowBoard(char** arr) {
    char alphacords[] = "a b c d e f g h";
    cout << "\t   " << alphacords << endl << endl;
    for (int i = 0; i < 8; i++) {
        cout << "\t" << 8 - i << "  ";
        for (int j = 0; j < 8; j++) {
            cout << arr[i][j] << " ";
        }
        cout << " " << 8 - i;
        cout << endl;
    }
    cout << endl << "\t   " << alphacords << endl << endl;
}

bool Find(char* arr, char needle) {
    int size = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < size; i++) {
        if (arr[i] == needle) {
            return true;
        }
        else if (arr[i] == '\0') {
            return false;
        }
    }
    return false;
}
void Find(char** arr, char needle, int* indexes) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (arr[i][j] == needle) {
                indexes[0] = i;
                indexes[1] = j;
                return;
            }
        }
    }
}
bool isKnightAttacking(char** arr, int x, int y, char turn, int* coords, bool mateCheck = false, int FixedRow = -100, int COL = -100, int newFixedRow = -100, int newCOL = -100) {
    int (*check)(int _C);

    int steps[8][2] = { {2, 1}, {1, 2}, {-1, 2}, {-2,  1},
                    {-2, -1}, {-1, -2}, {1, -2}, {2, -1} };
    if (mateCheck) {
        if (turn == 'w') {
            check = toupper;
        }
        else {
            check = tolower;
        }
    }
    else {
        if (turn == 'w') {
            check = tolower;
        }
        else {
            check = toupper;
        }
    }

    for (int i = 0; i < 8; i++) {
        int newX = steps[i][0];
        int newY = steps[i][1];
        if (newFixedRow == x + newX && newCOL == y + newY) {
            continue;
        }
        if ((newX + x < 0 || newX + x>7) || (newY + y < 0 || newY + y>7)) {
            continue;
        }
        if (arr[x + newX][y + newY] == check('n')) {
            if (mateCheck) {
                if (isPieceFree(arr, check('n'), x + newX, y + newY, turn, x, y, true)) {
                    coords[0] = x + newX; coords[1] = y + newY;
                    return true;
                }
            }
            else {
                coords[0] = x + newX; coords[1] = y + newY;
                return true;
            }
        }
    }
    return false;
}
bool CheckMateCombo(char** arr, char turn, int kingX, int kingY, int enemyX, int enemyY) {
    int (*check)(int _C);
    if (turn == 'w') {
        check = toupper;
    }
    else {
        check = tolower;
    }
    int possible_moves[9][2] = { {1,-1}, {1,0},{1,1},   //up
                                    {0,-1}, {0,1}, // same height
                                {-1,-1}, {-1,0}, {-1,1} };  //down

    int mateType = 0; // 1 - вертикальный 2 - горизонтальный 3 - диагональ 4 - кмат конем
    int start; int s;
    int end; int e;
    char king;
    bool canMove = false;
    int index1, index2;
    int startCOL, endCOL; char moveType;
    if (turn == 'w') {
        king = 'K';
    }
    else {
        king = 'k';
    }
    if (kingX == enemyX && kingY != enemyY || kingX != enemyX && kingY == enemyY) { //ладья/ферзь
        if (kingX == enemyX && kingY != enemyY) {//горизонатльный
            mateType = 2;
            start = enemyY; s = enemyY;
            end = kingY; e = kingY;
        }
        else if (kingX != enemyX && kingY == enemyY) {//вертикальный
            mateType = 1;
            start = enemyX; s = enemyX;
            end = kingX; e = kingX;
        }
    }
    else if (abs(kingX - enemyX) == abs(kingY - enemyY)) {//диагональ, слон\ферз
        mateType = 3;
        start = enemyX; s = enemyX;
        end = kingX; e = kingX;
        startCOL = enemyY;
        endCOL = kingY;
    }
    else if (abs(kingX - enemyX) == 2 && abs(kingY - enemyY) == 1 || abs(kingX - enemyX) == 1 && abs(kingY - enemyY) == 2) { //конь
        mateType = 4;
        start = enemyX; s = enemyX;
        end = kingX; e = kingX;
        startCOL = enemyY;
        endCOL = kingY;
    }

    for (int i = 0; i < 9; i++) { //iteratring all king possible moves
        if ((kingX + possible_moves[i][0] < 0 || kingX + possible_moves[i][0] > 7) || (kingY + possible_moves[i][1] < 0 || kingY + possible_moves[i][1] > 7) || (arr[kingX + possible_moves[i][0]][kingY + possible_moves[i][1]] != '=' && arr[kingX + possible_moves[i][0]][kingY + possible_moves[i][1]] == check(arr[kingX + possible_moves[i][0]][kingY + possible_moves[i][1]]))) {
            continue;
        }
        if (isPieceFree(arr, king, kingX, kingY, turn, kingX + possible_moves[i][0], kingY + possible_moves[i][1], true)) { //check if king can even move
            canMove = true;
            //return
        }
    }
    //k 3 // e 1
    // s, start = enemy
    //e, end = king
    bool canBeProtectedByPiece = false;
    int diagonal1 = 0;
    int diagonal2 = 0;
    int helpCoordinate, helpCoordinate2;
    int coords[2] = { 0,0 };
    while ((s > e) ? (start > end) : (start < end)) {//проход по каждому аттаковану полю
        /*  if (mateType == 1) {
              index1 = start;
              index2 = kingY;
          }*/
        helpCoordinate = start;
     
        if (mateType == 1) {
            helpCoordinate = start;
            helpCoordinate2 = kingY;
        }
        else if (mateType == 2) {
            helpCoordinate = kingX;
            helpCoordinate2 = start;
        }
        else if (mateType == 3 || mateType == 4) {
            helpCoordinate = start;
            helpCoordinate2 = startCOL;

        }
        // start = индекс i/j поля, end  = индекс короля
        for (int i = 0; i < 8; i++) { //вертикаль
            if (arr[i][helpCoordinate2] == check('r') || arr[i][helpCoordinate2] == check('q') || arr[i][helpCoordinate2] == check('p')) {
                if (arr[i][helpCoordinate2] == check('p')) {
                    if (turn == 'w'&& i-1 == helpCoordinate) {
                        if (isLegalMove(arr, arr[i][helpCoordinate2], i, helpCoordinate2,0, turn, ((arr[helpCoordinate][helpCoordinate2] == '=') ? '>' : ':'), helpCoordinate, helpCoordinate2, true)) {
                            canBeProtectedByPiece = true;
                        }
                    }
                    else if (turn == 'b' && i + 1 == helpCoordinate) {
                        
                        if (isLegalMove(arr, arr[i][helpCoordinate2], i, helpCoordinate2, 0, turn , ((arr[helpCoordinate][helpCoordinate2] == '=') ? '>' : ':'), helpCoordinate, helpCoordinate2, true)) {
                            canBeProtectedByPiece = true;
                        }
                    }
                }
                else {
                    if (isLegalMove(arr, arr[i][helpCoordinate2], i, helpCoordinate2, turn, ((arr[helpCoordinate][helpCoordinate2] == '=') ? '>' : ':'), helpCoordinate, helpCoordinate2, true)) {
                        canBeProtectedByPiece = true;
                    }
                }
            }
        }

        for (int i = 0; i < 8; i++) {//горизонатль

            if (arr[start][i] == check('r') || arr[start][i] == check('q')) {
                if (isLegalMove(arr, arr[start][i], start, i, turn, ((arr[start][kingY] == '=') ? '>' : ':'), start, kingY, true)) {
                    canBeProtectedByPiece = true;
                }
            }
            else if (arr[start][i] == check('p')) {
                if (turn == 'b') {
                    if (isLegalMove(arr, arr[start][i], start, i, turn, ((arr[start][start+1] == '=') ? '>' : ':'), start, start + 1, true)) {
                        canBeProtectedByPiece = true;
                    }
                }
                else if (turn == 'w') {
                    if (isLegalMove(arr, arr[start][i], start, i, turn, ((arr[start][start - 1] == '=') ? '>' : ':'), start, start - 1, true)) {
                        canBeProtectedByPiece = true;
                    }
                }
            }

        }
        diagonal1 = 0;
        diagonal2 = enemyY;
  /*      if (mateType == 1) {
            helpCoordinate = start;
            helpCoordinate2 = kingY;
        }
        else if (mateType == 2) {
            helpCoordinate = kingX;
            helpCoordinate2 = start;
        }
        else if (mateType == 3 || mateType == 4) {
            helpCoordinate = start;
            helpCoordinate2 = startCOL;

        }*/
       
        diagonal2 = enemyY;
        for (int i = start; i > -1; i--) { //диагонль 0 0   8 8
            if (arr[i][diagonal2] == check('b') || arr[i][diagonal2] == check('q')) {
                if (isLegalMove(arr, arr[i][diagonal2], i, diagonal2, turn, ((arr[helpCoordinate][helpCoordinate2] == '=') ? '>' : ':'), helpCoordinate, helpCoordinate2, true)) {
                    canBeProtectedByPiece = true;
                }
            }
            else if (arr[i][diagonal2] == check('p')) {
                if (abs(i - enemyX) == 1 && abs(diagonal2 - enemyY) == 1) {
                    if (isLegalMove(arr, arr[i][diagonal2], i, diagonal2, turn, ((arr[enemyX][enemyY] == '=') ? '>' : ':'), enemyX, enemyY, true)) {
                        canBeProtectedByPiece = true;
                    }
                }
            }
            diagonal2--;
        }
        diagonal2 = enemyY;

        for (int i = start; i < 8; i++) { //диагонль 0 0   8 8

            if (arr[i][diagonal2] == check('b') || arr[i][diagonal2] == check('q')) {
                if (isLegalMove(arr, arr[i][diagonal2], i, diagonal2, turn, ((arr[helpCoordinate][helpCoordinate2] == '=') ? '>' : ':'), helpCoordinate, helpCoordinate2, true)) {
                    canBeProtectedByPiece = true;
                }
            }
            else if (arr[i][diagonal2] == check('p')) {
                if (abs(i - enemyX) == 1 && abs(diagonal2 - enemyY) == 1) {
                    if (isLegalMove(arr, arr[i][diagonal2], i, diagonal2, turn, ((arr[enemyX][enemyY] == '=') ? '>' : ':'), enemyX, enemyY, true)) {
                        canBeProtectedByPiece = true;
                    }
                }
            }
            diagonal2--;
        }
        diagonal2 = enemyY;

        for (int i = start; i > -1; i--) { //диагонль 8 0   0 8
            if (arr[i][diagonal2] == check('b') || arr[i][diagonal2] == check('q')) {
                if (isLegalMove(arr, arr[i][diagonal2], i, diagonal2, turn, ((arr[helpCoordinate][helpCoordinate2] == '=') ? '>' : ':'), helpCoordinate, helpCoordinate2, true)) {
                    canBeProtectedByPiece = true;
                }
            }
            else if (arr[i][diagonal2] == check('p')) {
                if (abs(i - enemyX) == 1 && abs(diagonal2 - enemyY) == 1) {
                    if (isLegalMove(arr, arr[i][diagonal2], i, diagonal2, turn, ((arr[enemyX][enemyY] == '=') ? '>' : ':'), enemyX, enemyY, true)) {
                        canBeProtectedByPiece = true;
                    }
                }
            }
            diagonal2++;
        }
        diagonal2 = enemyY;
        for (int i = start; i < 8; i++) { //диагонль 8 0   0 8
            if (arr[i][diagonal2] == check('b') || arr[i][diagonal2] == check('q')) {
                if (isLegalMove(arr, arr[i][diagonal2], i, diagonal2, turn, ((arr[helpCoordinate][helpCoordinate2] == '=') ? '>' : ':'), helpCoordinate, helpCoordinate2, true)) {
                    canBeProtectedByPiece = true;
                }
            }
            else if (arr[i][diagonal2] == check('p')) {
                if (abs(i - enemyX) == 1 && abs(diagonal2 - enemyY) == 1) {
                    if (isLegalMove(arr, arr[i][diagonal2], i, diagonal2, turn, ((arr[enemyX][enemyY] == '=') ? '>' : ':'), enemyX, enemyY, true)) {
                        canBeProtectedByPiece = true;
                    }
                }
            }
            diagonal2++;
        }

        if (mateType == 1 || mateType == 2) { // 1 - вертикальный мат // 2 - горизонтальный
            if (mateType == 1) {
                if (isKnightAttacking(arr, start, kingY, turn, coords, true)) {
                    canBeProtectedByPiece = true;
                }
            }
            else if (mateType == 2) {
                if (isKnightAttacking(arr, kingX, start, turn, coords, true)) {
                    canBeProtectedByPiece = true;
                }
            }
            if (s > e) {
                start--;
            }
            else {
                start++;
            }
        }
        else if (mateType == 3) { //диагоналдб

            if (enemyY < kingY) {
                startCOL++;
            }
            else {
                startCOL--;
            }

            if (s > e) {
                start--;
            }
            else {
                start++;
            }
        }
        else if (mateType == 4) { //мат конем
            break;
        }
    }
    if (canMove == false && canBeProtectedByPiece == false) {
        return true; //mate!!!
    }
    else {
        return false; //not mate
    }
}

bool isPieceFree(char** arr, char piece, int FixedRow, int COL, char turn, int newFixedRow, int NewCOL, bool mateCheck) {
    int indexes[2];
    if (piece == 'm') {
        if (turn == 'w') {
            Find(arr, 'K', indexes);
        }
        else {
            Find(arr, 'k', indexes);
        }
        FixedRow = indexes[0];
        COL = indexes[1];
        newFixedRow = indexes[0];
        NewCOL = indexes[1];

    }
    else if (tolower(piece) == 'k') {
        indexes[0] = newFixedRow;
        indexes[1] = NewCOL;
    }
    else {
        if (turn == 'w') {
            Find(arr, 'K', indexes);
        }
        else {
            Find(arr, 'k', indexes);
        }
    }
    /*cout << indexes[0] << ' ' << indexes[1];*/
    int isFreeCount = 9;
    bool isRan = false;
    bool isFree = false;
    //вертикаль
    for (int i = indexes[0] + 1; i < 8; i++) {

        isRan = true;

        if (i == FixedRow && indexes[1] == COL) {
            continue;
        }
        if (i == newFixedRow && indexes[1] == NewCOL) {
            isFree = true;
            break;
        }
        else if (arr[i][indexes[1]] != '=') {
            if (turn == 'w' && (arr[i][indexes[1]] == 'r' || arr[i][indexes[1]] == 'q')) {
                isFree = false; isFreeCount--;
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, indexes[1])) {
                        return false; //mate
                    }
                }
                break;
            }
            else if (turn == 'b' && (arr[i][indexes[1]] == 'R' || arr[i][indexes[1]] == 'Q')) {
                isFree = false; isFreeCount--;
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, indexes[1])) {
                        return false; //mate
                    }
                }
                break;
            }
            else {
                isFree = true;
                break;
            }
        }
    }

    for (int i = indexes[0] - 1; i > -1; i--) {
        isRan = true;

        if (i == FixedRow && indexes[1] == COL) {
            continue;
        }
        if (i == newFixedRow && indexes[1] == NewCOL) {
            isFree = true;
            break;
        }
        else if (arr[i][indexes[1]] != '=') {
            if (turn == 'w' && (arr[i][indexes[1]] == 'r' || arr[i][indexes[1]] == 'q')) {
                isFree = false; isFreeCount--;
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, indexes[1])) {
                        return false; //mate
                    }
                }
                break;
            }
            else if (turn == 'b' && (arr[i][indexes[1]] == 'R' || arr[i][indexes[1]] == 'Q')) {
                isFree = false; isFreeCount--;
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, indexes[1])) {
                        return false; //mate
                    }
                }
                break;
            }
            else {
                isFree = true;
                break;
            }
        }

    }

    //горизонталь
    for (int j = indexes[1] + 1; j < 8; j++) {
        isRan = true;
        if (indexes[0] == FixedRow && j == COL) {
            continue;
        }
        if (indexes[0] == newFixedRow && j == NewCOL) {
            isFree = true;
            break;
        }
        else if (arr[indexes[0]][j] != '=') {
            if (turn == 'w' && (arr[indexes[0]][j] == 'r' || arr[indexes[0]][j] == 'q')) {
                isFree = false; isFreeCount--;
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], indexes[0], j)) {
                        return false; //mate
                    }
                }
                break;
            }
            else if (turn == 'b' && (arr[indexes[0]][j] == 'R' || arr[indexes[0]][j] == 'Q')) {
                isFree = false; isFreeCount--;
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], indexes[0], j)) {
                        return false; //mate
                    }
                }
                break;
            }
            else {
                isFree = true;
                break;
            }
        }
    }

    for (int j = indexes[1] - 1; j > -1; j--) {
        isRan = true;
        if (indexes[0] == FixedRow && j == COL) {
            continue;
        }
        if (indexes[0] == newFixedRow && j == NewCOL) {
            isFree = true;
            break;
        }
        else if (arr[indexes[0]][j] != '=') {
            if (turn == 'w' && (arr[indexes[0]][j] == 'r' || arr[indexes[0]][j] == 'q')) {
                isFree = false; isFreeCount--;
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], indexes[0], j)) {
                        return false; //mate
                    }
                }
                break;
            }
            else if (turn == 'b' && (arr[indexes[0]][j] == 'R' || arr[indexes[0]][j] == 'Q')) {
                isFree = false; isFreeCount--;
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], indexes[0], j)) {
                        return false; //mate
                    }
                }
                break;
            }
            else {
                isFree = true;
                break;
            }
        }

    }
    //диагональ kX kY  (0 0) = 8 8
    int diagonal = indexes[1] + 1;
    //1
    for (int i = indexes[0] + 1; i < 8; i++) { //1
        isRan = true;
        if (i == FixedRow && diagonal == COL) {
            diagonal++;
            continue;
        }
        if (i == newFixedRow && diagonal == NewCOL) {
            isFree = true;
            break;
        }
        else if (arr[i][diagonal] != '=') {
            if (turn == 'w' && (arr[i][diagonal] == 'b' || arr[i][diagonal] == 'q' || (arr[i][diagonal] == 'p' && (i < indexes[0] && (abs(diagonal - indexes[1]) == 1))))) {

                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, diagonal)) {
                        return false; //mate
                    }
                }
                isFree = false; isFreeCount--;
                break;

            }
            else if (turn == 'b' && (arr[i][diagonal] == 'B' || arr[i][diagonal] == 'Q' || (arr[i][diagonal] == 'P' && (i > indexes[0] && (abs(diagonal - indexes[1]) == 1))))) {

                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, diagonal)) {
                        return false; //mate
                    }
                }
                isFree = false; isFreeCount--;
                break;

            }
            else {
                isFree = true;
                break;
            }
        }
        diagonal++;
    }

    diagonal = indexes[1] - 1;
    //2
    for (int i = indexes[0] + 1; i < 8; i++) { //2
        isRan = true;
        if (i == FixedRow && diagonal == COL) {
            diagonal--;
            continue;
        }
        if (i == newFixedRow && diagonal == NewCOL) {
            isFree = true;
            break;
        }
        else if (arr[i][diagonal] != '=') {
            if (turn == 'w' && (arr[i][diagonal] == 'b' || arr[i][diagonal] == 'q' || (arr[i][diagonal] == 'p' && (i < indexes[0] && (abs(diagonal - indexes[1]) == 1))))) {

                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, diagonal)) {
                        return false; //mate
                    }
                }
                isFree = false; isFreeCount--;
                break;

            }
            else if (turn == 'b' && (arr[i][diagonal] == 'B' || arr[i][diagonal] == 'Q' || (arr[i][diagonal] == 'P' && (i > indexes[0] && (abs(diagonal - indexes[1]) == 1))))) {
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, diagonal)) {
                        return false; //mate
                    }
                }
                isFree = false; isFreeCount--;
                break;

            }
            else {
                isFree = true;
                break;
            }
        }
        diagonal--;
    }
    //конец 1 диагонали
    //диагональ Kx Ky (8 0) = 0 8
    diagonal = indexes[1] + 1;
    for (int i = indexes[0] - 1; i > -1; i--) {
        isRan = true;
        if (i == FixedRow && diagonal == COL) {
            diagonal++;
            continue;
        }
        if (i == newFixedRow && diagonal == NewCOL) {
            isFree = true;
            break;
        }
        else if (arr[i][diagonal] != '=') {
            if (turn == 'w' && (arr[i][diagonal] == 'b' || arr[i][diagonal] == 'q' || (arr[i][diagonal] == 'p' && (i < indexes[0] && (abs(diagonal - indexes[1]) == 1))))) {

                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, diagonal)) {
                        return false; //mate
                    }
                }
                isFree = false; isFreeCount--;
                break;

            }
            else if (turn == 'b' && (arr[i][diagonal] == 'B' || arr[i][diagonal] == 'Q' || (arr[i][diagonal] == 'P' && (i > indexes[0] && (abs(diagonal - indexes[1]) == 1))))) {
                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, diagonal)) {
                        return false; //mate
                    }
                }
                isFree = false; isFreeCount--;
                break;
            }
            else {
                isFree = true;
                break;
            }
        }
        diagonal++;
    }
    //2
    diagonal = indexes[1] - 1;
    for (int i = indexes[0] - 1; i > -1; i--) {
        isRan = true;
        if (i == FixedRow && diagonal == COL) {
            diagonal--;
            continue;
        }
        if (i == newFixedRow && diagonal == NewCOL) {
            isFree = true;
            break;
        }
        else if (arr[i][diagonal] != '=') {
            if (turn == 'w' && (arr[i][diagonal] == 'b' || arr[i][diagonal] == 'q' || (arr[i][diagonal] == 'p' && (i < indexes[0] && (abs(diagonal - indexes[1]) == 1))))) {

                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, diagonal)) {
                        return false; //mate
                    }
                }
                isFree = false; isFreeCount--;
                break;

            }
            else if (turn == 'b' && (arr[i][diagonal] == 'B' || arr[i][diagonal] == 'Q' || (arr[i][diagonal] == 'P' && (i > indexes[0] && (abs(diagonal - indexes[1]) == 1))))) {

                if (piece == 'm') {
                    if (CheckMateCombo(arr, turn, indexes[0], indexes[1], i, diagonal)) {
                        return false; //mate
                    }
                }
                isFree = false; isFreeCount--;
                break;

            }
            else {
                isFree = true;
                break;
            }
        }
        diagonal--;
    }
    //конец 2 диагонали
    int coords[2] = { 0,0 };
    if (isKnightAttacking(arr, indexes[0], indexes[1], turn, coords, false, FixedRow, COL, newFixedRow, NewCOL)) {
        if (piece == 'm') {
            if (CheckMateCombo(arr, turn, indexes[0], indexes[1], coords[0], coords[1])) {
                return false; //mate
            }
        }
        isFreeCount--;
    }
    if (isFreeCount == 9) {
        return true;
    }
    else {
        if (piece == 'm') {
            return true;
        }
        if (mateCheck == false) {
            cout << endl << "Нелегальный ход! Ваша король находится под шахом, либо фигура которую вы хотите походить,защищает короля от нападения!" << endl;
        }
        return false;
    }
}

bool isLegalMove(char** arr, char piece, int FixedRow, int COL, int fieldSide, char turn, char moveType, int newFixedRow, int NewCOL, bool mateCheck) {
    if (turn == 'w') {
        piece = toupper(piece);
    }
    else {
        piece = tolower(piece);
    }

    switch (toupper(piece)) {
    case 'K':
        if (((FixedRow == newFixedRow && COL != NewCOL && abs(COL - NewCOL) == 1 || (FixedRow != newFixedRow && COL == NewCOL && abs(FixedRow - newFixedRow) == 1))) || (abs(FixedRow - newFixedRow) == 1 && abs(COL - NewCOL) == 1)) {
            if (arr[FixedRow][COL] == piece) {

                if (moveType == '>' && arr[newFixedRow][NewCOL] == '=') {
                    return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);

                }
                else if (moveType == ':' && arr[newFixedRow][NewCOL] != '=') {
                    if (turn == 'w' && islower(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                    else if (turn == 'b' && isupper(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                }
            }
        }
        break;
    case 'P': //pawn 
        if (moveType == '>') {
            if ((arr[FixedRow][COL] == '=') && (arr[FixedRow + (fieldSide)][COL] == (char)tolower(piece) || arr[FixedRow + (fieldSide)][COL] == (char)toupper(piece))) { //проверка если ход легален (если пешка есть на поле)
                return isPieceFree(arr, piece, FixedRow + (fieldSide), COL, turn, FixedRow, COL, mateCheck);
            }
        }
        else {
            if (arr[FixedRow][COL] == piece && (abs(FixedRow - newFixedRow) == 1 && abs(COL - NewCOL) == 1)) { //проверка если ход легален 

                if (turn == 'w' && islower(arr[newFixedRow][NewCOL])) {
                    return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                }
                else if (turn == 'b' && isupper(arr[newFixedRow][NewCOL])) {
                    return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                }
            }
        }
        break;
    case 'N': //конь
        if (moveType == '>') {
            if ((arr[FixedRow][COL] == piece) && (abs(newFixedRow - FixedRow) == 2 && abs(NewCOL - COL) == 1 || abs(newFixedRow - FixedRow) == 1 && abs(NewCOL - COL) == 2)) {
                if (arr[newFixedRow][NewCOL] == '=') {
                    return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                }
            }
        }
        else {
            if (arr[FixedRow][COL] == piece && arr[newFixedRow][NewCOL] != '=') { //проверка если ход легален 
                if (turn == 'w' && islower(arr[newFixedRow][NewCOL])) {
                    return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                }
                else if (turn == 'b' && isupper(arr[newFixedRow][NewCOL])) {
                    return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                }
            }
        }
        break;
    case 'R':
        if ((arr[FixedRow][COL] == piece) && (FixedRow == newFixedRow && COL != NewCOL || FixedRow != newFixedRow && COL == NewCOL)) {
            if (FixedRow == newFixedRow && COL != NewCOL) {// горизотналь
                if (COL - NewCOL > 0) {
                    for (int i = COL - 1; i > NewCOL; i--) { //справа на лево
                        if (arr[newFixedRow][i] != '=') {
                            return false;
                        }
                    }
                }
                else if (COL - NewCOL < 0) {
                    for (int i = COL + 1; i < NewCOL; i++) { //сслоева направо
                        if (arr[newFixedRow][i] != '=') {
                            return false;
                        }
                    }
                }
            }

            else if (FixedRow != newFixedRow && COL == NewCOL) { // вертикаль
                if (FixedRow - newFixedRow > 0) {
                    for (int i = FixedRow - 1; i > newFixedRow; i--) {//сверзу вниз
                        if (arr[i][NewCOL] != '=') {
                            return false;
                        }
                    }
                }
                else if (FixedRow - newFixedRow < 0) {
                    for (int i = newFixedRow - 1; i > FixedRow; i--) {//снизу вверх
                        if (arr[i][NewCOL] != '=') {
                            return false;
                        }
                    }
                }
            }
            //Cheking if rook attakicg
            if (moveType == ':') {
                if (arr[FixedRow][COL] == piece) { //проверка если ход легален 

                    if (turn == 'w' && islower(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                    else if (turn == 'b' && isupper(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                    else {
                        return false;
                    }
                }
            }
            else if ((moveType == '>') && (arr[newFixedRow][NewCOL] != '=')) {
                return false;
            }
            return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
        }
        break;
    case 'B':
        if ((arr[FixedRow][COL] == piece) && (abs(FixedRow - newFixedRow) == abs(COL - NewCOL))) {
            int COLhelp = COL;
            if (COL < NewCOL) {// 1 диагонль (право)
                COLhelp++;
                if (FixedRow > newFixedRow) {
                    for (int i = FixedRow - 1; i > newFixedRow; i--) { //вниз
                        if (arr[i][COLhelp] != '=') {
                            return false;
                        }
                        COLhelp++;
                    }
                }
                else if (FixedRow < newFixedRow) {
                    for (int i = FixedRow + 1; i < newFixedRow; i++) { //вверх
                        if (arr[i][COLhelp] != '=') {
                            return false;
                        }
                        COLhelp++;
                    }
                }
            }
            else if (COL > NewCOL) { // в2 диагональ (лево)
                COLhelp--;
                if (FixedRow > newFixedRow) {
                    for (int i = FixedRow - 1; i > newFixedRow; i--) { //вниз
                        if (arr[i][COLhelp] != '=') {
                            return false;
                        }
                        COLhelp--;
                    }
                }
                else if (FixedRow < newFixedRow) {
                    for (int i = FixedRow + 1; i < newFixedRow; i++) { //вверх
                        if (arr[i][COLhelp] != '=') {
                            return false;
                        }
                        COLhelp--;
                    }
                }
            }
            if (moveType == ':') {
                if (arr[FixedRow][COL] == piece) { //проверка если ход легален 

                    if (turn == 'w' && islower(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                    else if (turn == 'b' && isupper(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                    else {
                        return false;
                    }
                }
            }
            else if ((moveType == '>') && (arr[newFixedRow][NewCOL] != '=')) {
                return false;
            }
            return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL);
        }
        break;
    case 'Q':
        if ((arr[FixedRow][COL] == piece) && (FixedRow == newFixedRow && COL != NewCOL || FixedRow != newFixedRow && COL == NewCOL)) { //линейный
            if (FixedRow == newFixedRow && COL != NewCOL) {// горизотналь
                if (COL - NewCOL > 0) {
                    for (int i = COL - 1; i > NewCOL; i--) { //справа на лево
                        if (arr[newFixedRow][i] != '=') {
                            return false;
                        }
                    }
                }
                else if (COL - NewCOL < 0) {
                    for (int i = COL + 1; i < NewCOL; i++) { //сслоева направо
                        if (arr[newFixedRow][i] != '=') {
                            return false;
                        }
                    }
                }
            }

            else if (FixedRow != newFixedRow && COL == NewCOL) { // вертикаль
                if (FixedRow - newFixedRow > 0) {
                    for (int i = FixedRow - 1; i > newFixedRow; i--) {//сверзу вниз
                        if (arr[i][NewCOL] != '=') {
                            return false;
                        }
                    }
                }
                else if (FixedRow - newFixedRow < 0) {
                    for (int i = newFixedRow - 1; i > FixedRow; i--) {//снизу вверх
                        if (arr[i][NewCOL] != '=') {
                            return false;
                        }
                    }
                }
            }
            //Cheking if rook attakicg
            if (moveType == ':') {
                if (arr[FixedRow][COL] == piece) { //проверка если ход легален 
                    if (turn == 'w' && islower(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                    else if (turn == 'b' && isupper(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                    else {
                        return false;
                    }
                }
            }
            else if ((moveType == '>') && (arr[newFixedRow][NewCOL] != '=')) {
                return false;
            }
            return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
        }
        else  if ((arr[FixedRow][COL] == piece) && (abs(FixedRow - newFixedRow) == abs(COL - NewCOL))) { //диагональный ход
            int COLhelp = COL;
            if (COL < NewCOL) {// 1 диагонль (право)
                COLhelp++;
                if (FixedRow > newFixedRow) {
                    for (int i = FixedRow - 1; i > newFixedRow; i--) { //вниз
                        if (arr[i][COLhelp] != '=') {
                            return false;
                        }
                        COLhelp++;
                    }
                }
                else if (FixedRow < newFixedRow) {
                    for (int i = FixedRow + 1; i < newFixedRow; i++) { //вверх
                        if (arr[i][COLhelp] != '=') {
                            return false;
                        }
                        COLhelp++;
                    }
                }
            }
            else if (COL > NewCOL) { // в2 диагональ (лево)
                COLhelp--;
                if (FixedRow > newFixedRow) {
                    for (int i = FixedRow - 1; i > newFixedRow; i--) { //вниз
                        if (arr[i][COLhelp] != '=') {
                            return false;
                        }
                        COLhelp--;
                    }
                }
                else if (FixedRow < newFixedRow) {
                    for (int i = FixedRow + 1; i < newFixedRow; i++) { //вверх
                        if (arr[i][COLhelp] != '=') {
                            return false;
                        }
                        COLhelp--;
                    }
                }
            }
            if (moveType == ':') {
                if (arr[FixedRow][COL] == piece) { //проверка если ход легален 
                    if (turn == 'w' && islower(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                    else if (turn == 'b' && isupper(arr[newFixedRow][NewCOL])) {
                        return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
                    }
                    else {
                        return false;
                    }
                }
            }
            else if ((moveType == '>') && (arr[newFixedRow][NewCOL] != '=')) {
                return false;
            }
            return isPieceFree(arr, piece, FixedRow, COL, turn, newFixedRow, NewCOL, mateCheck);
        }
        break;
    }
    return false;
}

bool Move(char** arr, char move[], char turn) {
    int ROW = (move[1] - '0'); //ЦИФРА
    int COL = 0; //БУКВА
    int FixedRow = 8 - ROW;
    int fieldSide = -1;
    char alphacords[] = "abcdefgh";

    //for not default move
    int NewCOL = 0;
    if (turn == 'w') {
        fieldSide = 1;
    }

    if (Find(move, ':')) { //attacking
        if (strlen(move) == 5) { //pawn attack

            int NewFixedRow = 8 - (move[4] - '0');


            if (FixedRow < 0 || 7 < FixedRow) {
                cout << "ILLEGAL MOVE!!!" << endl;
                return false;
            }

            COL = tolower(move[0]) - 'a';
            NewCOL = tolower(move[3]) - 'a';

            if (isLegalMove(arr, 'P', FixedRow, COL, fieldSide, turn, ':', NewFixedRow, NewCOL)) { //проверка на легальность аттаки
                switch (turn) {
                case 'w': //white
                    arr[FixedRow][COL] = '=';
                    arr[NewFixedRow][NewCOL] = 'P';
                    return true;
                case 'b': //black
                    arr[FixedRow][COL] = '=';
                    arr[NewFixedRow][NewCOL] = 'p';
                    return true;
                };
            }
            else {
                cout << "Illegal move!!! Try again!" << endl;
                return false;
            }


        }
        else if (strlen(move) == 6) {
            FixedRow = 8 - (move[2] - '0');
            int NewFixedRow = 8 - (move[5] - '0');
            char piece = move[0];

            if (FixedRow < 0 || 7 < FixedRow) {
                cout << "ILLEGAL MOVE!!!" << endl;
                return false;
            }

            COL = tolower(move[1]) - 'a';
            NewCOL = tolower(move[4]) - 'a';

            if (isLegalMove(arr, piece, FixedRow, COL, fieldSide, turn, move[3], NewFixedRow, NewCOL)) { //проверка на легальность аттаки
                switch (turn) {
                case 'w': //white
                    arr[FixedRow][COL] = '=';
                    arr[NewFixedRow][NewCOL] = toupper(piece);
                    return true;
                case 'b': //black
                    arr[FixedRow][COL] = '=';
                    arr[NewFixedRow][NewCOL] = tolower(piece);
                    return true;
                };
            }
            else {
                cout << "Illegal move!!! Try again!" << endl;
                return false;
            }

        }
    }
    else if (strlen(move) == 2) { //pawn default move

        if (FixedRow < 0 || 7 < FixedRow) {
            cout << "ILLEGAL MOVE!!!" << endl;
            return false;
        }

        COL = tolower(move[0]) - 'a';

        if (isLegalMove(arr, 'P', FixedRow, COL, fieldSide, turn, '>')) { //проверка на легальность хода
            switch (turn) {
            case 'w': //white
                arr[FixedRow + (fieldSide)][COL] = '=';
                arr[FixedRow][COL] = 'P';
                return true;
            case 'b': //black
                arr[FixedRow + (fieldSide)][COL] = '=';
                arr[FixedRow][COL] = 'p';
                return true;
            };
        }
        else {
            cout << "Illegal move!!! Try again!" << endl;
            return false;
        }


    }
    else if (strlen(move) == 6) {
        FixedRow = 8 - (move[2] - '0');
        int NewFixedRow = 8 - (move[5] - '0');
        char piece = move[0];

        if (FixedRow < 0 || 7 < FixedRow) {
            cout << "ILLEGAL MOVE!!!" << endl;
            return false;
        }

        COL = tolower(move[1]) - 'a';
        NewCOL = tolower(move[4]) - 'a';

        if (isLegalMove(arr, piece, FixedRow, COL, fieldSide, turn, move[3], NewFixedRow, NewCOL)) { //проверка на легальность аттаки
            switch (turn) {
            case 'w': //white
                arr[FixedRow][COL] = '=';
                arr[NewFixedRow][NewCOL] = toupper(piece);
                return true;
            case 'b': //black
                arr[FixedRow][COL] = '=';
                arr[NewFixedRow][NewCOL] = tolower(piece);
                return true;
            };
        }
        else {
            cout << "Illegal move!!! Try again!" << endl;
            return false;
        }

    }
    else {
        return false;
    }
    return false;
}

void main()
{
    //БЕЛЫЕ
    //ПУСТАЯ КЛЕТКА = "=", ПЕШКА = P,КОНЬ = N, СЛОН = B, ЛАДЬЯ = R, ФЕРЗЬ = Q , КОРОЛЬ = K 

    // ЧЕРНЫЕ
    //ПУСТАЯ КЛЕТКА = "=", ПЕШКА = p,КОНЬ = n, СЛОН = b, ЛАДЬЯ = r, ФЕРЗЬ = q , КОРОЛЬ = k 
    setlocale(LC_ALL, "RU");
    char** board = new char* [8];
    for (int i = 0; i < 8; i++) {
        board[i] = new char[8];
    }
    generate(board);
    ShowBoard(board);

    char mov[8];
    char move = 'w';
    while (true) {
        cin >> mov;
        if (Move(board, mov, move)) { //делаем ход и проверяем если он был легальный
            ShowBoard(board);
            if (move == 'w') {
                move = 'b';
            }
            else {
                move = 'w';
            }
            if (isPieceFree(board, 'm', 0, 0, move, 0, 0) == false) {
                cout << "МАТ!!!" << endl;
                break;
            }
        }
    }
}