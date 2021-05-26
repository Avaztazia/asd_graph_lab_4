#include<windows.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define numb 11
#define oneLen 180

int nx[numb] = {};
int ny[numb] = {};
double** mat = NULL;
double** mirror = NULL;
double** tree = NULL;
int check = 0;

void pausa ( HWND* hWnd )
{
    printf("\nClick to continue\n");
    while (1)
    {
		if (_kbhit()) {
			char c = _getch();
			InvalidateRect(*hWnd, NULL, TRUE);
			break;
		}
	}
}

double** randmm(int rows, int cols)
{
    double** matrix = (double**)malloc(rows * sizeof(double*));

    for (int i = 0; i < rows; i++)
        matrix[i] = (double*)malloc(cols * sizeof(double));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] =  2.0 / RAND_MAX * rand();
        }
    }

    return matrix;
}

double** multip (double **fMat, double **Smat, double **Rmat, int rows, int cols)
{
    for ( int i = 0; i < rows; i++ )
    {
        for ( int j = 0; j < cols; j++ )
        {
            Rmat[i][j] = 0;
            for ( int k = 0; k < cols; k++ )
            {
                Rmat[i][j] += fMat[i][k] * Smat[k][j];
            }
        }
    }
    return Rmat;
}

double** transp (double **mat, double **Rmat, int rows, int cols)
{
    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
            Rmat[i][j] = 0;
        }
    }
    for ( int i = 0; i < rows; i++ ) {
        for ( int j = 0; j < cols; j++ ) {
            Rmat[j][i] = mat[i][j];
        }
    }
    return Rmat;
}

double** mulmr(double num, double **mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            mat[i][j] = mat[i][j] * num;

            if(mat[i][j] > 1.0)
            {
                mat[i][j] = 1;
            } else mat[i][j] = 0;
        }
    }

    return mat;
}

void drawGraph ( HDC hdc, double** matrix )
{
    void arrow(float fi, int px,int py){
            fi = 3.1416*(180.0 - fi)/180.0;
            int lx,ly,rx,ry;
            lx = px+15*cos(fi+0.3);
            rx = px+15*cos(fi-0.3);
            ly = py+15*sin(fi+0.3);
            ry = py+15*sin(fi-0.3);
            MoveToEx(hdc, lx, ly, NULL);
            LineTo(hdc, px, py);
            LineTo(hdc, rx, ry);
    }
    char *nn[numb] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b"};
    int dx = 16, dy = 16, dtx = 7;
    int i;
    HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
    HPEN current = CreatePen(PS_SOLID, 1, RGB(255, 250, 48));
    HPEN finished = CreatePen(PS_SOLID, 1, RGB(128, 255, 48));
    SelectObject(hdc, KPen);
    for (int i = 0; i < numb; i++)
            {
                for (int j = 0; j < numb; j++)
                {
                    if ( matrix[i][j] == 1 ) {
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        if ( ( ( nx[i] == nx[j] ) && ( ( ny[i] - ny[j] == oneLen ) || ( ny[j] - ny[i] == oneLen ) ) ) != 1 && ( ( ny[i] == ny[j] ) && ( ( nx[i] - nx[j] == oneLen ) || ( nx[j] - nx[i] == oneLen ) ) ) != 1 && ( ( i == 0 && j == ( numb - 1 ) ) ||  ( j == 0 && i == ( numb - 1 ) ) ) != 1 ) {
                            if ( i == j ) {

                            }
                            else {
                                POINT pt[3];
                                pt[0].x = nx[i];
                                pt[0].y = ny[i];
                                pt[2].x = nx[j];
                                pt[2].y = ny[j];

                                if ( nx[i] == nx[j] ){

                                    switch ( i - j ) {
                                    case 2: case -2:
                                        if ( nx[i] == oneLen ) {
                                            pt[0].x -= 15;
                                            pt[2].x -= 15;
                                            pt[1].x = nx[i] - oneLen/2;
                                            if ( ny[i] > ny[j] ) {
                                                pt[1].y = ny[i] - oneLen;
                                                arrow(60, nx[j]-dx, ny[j]+dy);
                                            }
                                            else {
                                                pt[1].y = ny[i] + oneLen;
                                                arrow(-60, nx[j]-dx, ny[j]-5);
                                            }
                                        }
                                        else {
                                            pt[0].x += 15;
                                            pt[2].x += 15;
                                            pt[1].x = nx[i] + oneLen;
                                            if ( ny[i] > ny[j] ) {
                                                pt[1].y = ny[i] - oneLen;
                                                arrow(150, nx[j]+dx, ny[j]+dy);
                                            }
                                            else {
                                                pt[1].y = ny[i] + oneLen;
                                                arrow(-150, nx[j]+dx, ny[j]-5);
                                            }
                                        }
                                        Polyline(hdc,pt,3);
                                        break;
                                    default:
                                        LineTo( hdc, nx[j], ny[j] );
                                        if ( ny[i] > ny[j] ) {
                                            arrow(90,nx[j],ny[j]+dy);
                                        }
                                        else {
                                            arrow(-90,nx[j],ny[j]-dy);
                                        }
                                        break;
                                    }

                                }
                                else if ( ny[i] == ny[j] ) {

                                    if ( ny[i] == oneLen || ny[i] == oneLen*3 ) {
                                        pt[1].y = ny[i];
                                        pt[1].x = nx[i];
                                        switch ( nx[i] - nx[j] ) {
                                        case 180*2: case 180*(-2):
                                            if ( ny[i] == oneLen ) {
                                                pt[1].y -= oneLen/2;
                                                if ( nx[i] > nx[j] ) {
                                                    pt[1].x -= oneLen;
                                                    arrow(35,nx[j]+15,ny[j]+6);
                                                }
                                                else {
                                                    pt[1].x += oneLen;
                                                    arrow(-35,nx[j]-15,ny[j]+6);
                                                }
                                            }
                                            else {
                                                pt[1].y += oneLen/2;
                                                if ( nx[i] > nx[j] ) {
                                                    pt[1].x -= oneLen;
                                                    arrow(145,nx[j]+15,ny[j]+6);

                                                }
                                                else {
                                                    pt[1].x += oneLen;
                                                    arrow(-145,nx[j]-15,ny[j]+6);
                                                }
                                            }
                                            break;
                                        case 180*3: case 180*(-3):
                                            if ( nx[i] > nx[j] ) {
                                                pt[0].x += 15;
                                                pt[2].x -= 15;
                                            }
                                            else {
                                                pt[0].x -= 15;
                                                pt[2].x += 15;
                                            }
                                            if ( ny[i] == oneLen ){
                                                pt[1].y -= oneLen/4*2;
                                                if ( nx[i] > nx[j] ) {
                                                    pt[1].x -= oneLen*3/2;
                                                    arrow(-160,nx[j]+15,ny[j]-9);
                                                }
                                                else {
                                                    pt[1].x += oneLen*3/2;
                                                    arrow(160,nx[j]+15,ny[j]-9);
                                                }
                                            }
                                            else {
                                                pt[1].y += oneLen/4*3;
                                                if ( nx[i] > nx[j] ) {
                                                    pt[1].x -= oneLen*3/2;
                                                    arrow(155,nx[j]+9,ny[j]+11);
                                                }
                                                else {
                                                    pt[1].x += oneLen*3/2;
                                                    arrow(-155,nx[j]+9,ny[j]+11);
                                                }
                                            }
                                            break;
                                        case 180*4: case 180*(-4):
                                            if ( nx[i] > nx[j] ) {
                                                pt[0].x += 13;
                                                pt[2].x -= 13;
                                            }
                                            else {
                                                pt[0].x -= 13;
                                                pt[2].x += 13;
                                            }
                                            if ( ny[i] == oneLen ) {
                                                pt[0].y -= 5;
                                                pt[2].y -= 5;
                                                pt[1].y -= oneLen;
                                                if ( nx[i] > nx[j] ) {
                                                    pt[1].x -= oneLen*2;
                                                    arrow(-160,nx[j]+9,ny[j]-15);
                                                }
                                                else {
                                                    pt[1].x += oneLen*2;
                                                    arrow(160,nx[j]+9,ny[j]-15);
                                                }
                                            }
                                            else {
                                                pt[0].y += 5;
                                                pt[2].y += 5;
                                                pt[1].y += oneLen*5/4;
                                                if ( nx[i] > nx[j] ) {
                                                    pt[1].x -= oneLen*2;

                                                }
                                                else {
                                                    pt[1].x += oneLen*2;
                                                    arrow(40,nx[j]-5,ny[j]+15);
                                                }
                                            }
                                            break;
                                        }
                                        Polyline(hdc,pt,3);
                                    }
                                    else {
                                        LineTo( hdc, nx[j], ny[j] );
                                        if ( nx[i] > nx[j] ) {
                                            arrow(180,nx[j]+dx,ny[j]);
                                        }
                                        else {
                                            arrow(0,nx[j]-dx,ny[j]);
                                        }
                                    }

                                }
                                else {
                                    //S..t starts here:
                                    if ( matrix[j][i] ) {

                                        if ( i - j < 0 ) {
                                            pt[1].x = nx[i] + 20;
                                            pt[1].y = ny[i] - oneLen;
                                            LineTo( hdc, nx[j], ny[j] );
                                            arrow(70,nx[j]-8,ny[j]+dy);
                                            Polyline(hdc,pt,3);
                                            arrow(-100,nx[i]+1,ny[i]-dy);
                                        }

                                    }
                                    else{
                                        if ( ny[i] - ny[j] == oneLen*2 ||  ny[j] - ny[i] == oneLen*2 ) {
                                            switch ( nx[i]-nx[j] ) {
                                            case oneLen:
                                                if ( ny[j] > ny[i]) arrow(70,nx[j]+9,ny[j]-dy);
                                                else arrow(-70,nx[j]+9,ny[j]+dy);
                                                break;
                                            case -oneLen:
                                                if ( ny[j] > ny[i]) arrow(-70,nx[j]-9,ny[j]-dy);
                                                else arrow(70,nx[j]-9,ny[j]+dy);
                                                break;
                                            case oneLen*2:
                                                if ( ny[j] > ny[i]) arrow(-130,nx[j]+12,ny[j]-dy+5);
                                                else arrow(130,nx[j]+12,ny[j]+dy-5);
                                                break;
                                            case -oneLen*2:
                                                if ( ny[j] > ny[i]) arrow(-50,nx[j]-12,ny[j]-dy+5);
                                                else arrow(50,nx[j]-12,ny[j]+dy-5);
                                                break;
                                            case oneLen*3:
                                                if ( ny[j] > ny[i]) arrow(-140,nx[j]+15,ny[j]-dy+7);
                                                else arrow(140,nx[j]+15,ny[j]+dy-7);
                                                break;
                                            case -oneLen*3:
                                                if ( ny[j] > ny[i]) arrow(50,nx[j]-15,ny[j]-dy+7);
                                                else arrow(-50,nx[j]-15,ny[j]+dy-7);
                                                break;
                                            case oneLen*4:
                                                if ( ny[j] > ny[i]) arrow(-150,nx[j]+11,ny[j]-dy+2);
                                                else arrow(140,nx[j]+15,ny[j]+dy-7);
                                                break;
                                            case -oneLen*4:
                                                if ( ny[j] > ny[i]) arrow(30,nx[j]-11,ny[j]-dy+2);
                                                else arrow(-40,nx[j]-15,ny[j]+dy-7);
                                                break;
                                            }
                                        }
                                        else {

                                            if ( nx[i] - nx[j] == oneLen ) {
                                                arrow(-135,nx[j]+10,ny[j]-10);
                                            }
                                            else if ( nx[j] - nx[i] == oneLen ) {
                                                arrow(45,nx[j],ny[j]-10);
                                            }
                                            else if ( nx[i] - nx[j] == oneLen*3 ){
                                                if ( ny[i] > ny[j]) arrow(165,nx[j]+14,ny[j]+5);
                                                else arrow(-165,nx[j]+14,ny[j]-5);
                                            }

                                        }
                                        LineTo( hdc, nx[j], ny[j] );
                                    }
                                }
                            }
                        }
                        else {

                            if ( matrix[j][i] ) {
                                if ( i - j > 0 ) {
                                    POINT pt[3];
                                    pt[0].x = nx[i];
                                    pt[0].y = ny[i];

                                    pt[2].x = nx[j];
                                    pt[2].y = ny[j];

                                    LineTo( hdc, nx[j], ny[j] );
                                    if ( nx[i] == nx[j] && ny[i] > ny[j] ) {
                                        arrow(90, nx[j], ny[j]+dy);
                                        pt[1].x = nx[i]-oneLen/4;
                                        pt[1].y = ny[i]-oneLen/2;
                                        arrow(60, nx[j]-7, ny[j]+dy);
                                    }
                                    else if ( nx[i] == nx[j] && ny[i] < ny[j] ) {
                                        arrow(-90, nx[j], ny[j]-dy);
                                        pt[1].x = nx[i]+oneLen/4;
                                        pt[1].y = ny[i]+oneLen/2;
                                        arrow(-60, nx[j]+7, ny[j]+dy);
                                    }
                                    else if ( nx[i] > nx[j] ) {
                                        arrow(180, nx[j]+dx, ny[j]);
                                        pt[1].x = nx[i]-oneLen/4;
                                        pt[1].y = ny[i]+oneLen/2;
                                        arrow(150, nx[j]+dx, ny[j]+7);
                                    }
                                    else {
                                        arrow(0, nx[j]-dx, ny[j]);
                                        pt[1].x = nx[i]+oneLen/4;
                                        pt[1].y = ny[i]+oneLen/2;
                                        arrow(-150, nx[j]+dx, ny[j]-7);
                                    }
                                    Polyline(hdc,pt,3);
                                }
                            }
                            else {
                                LineTo( hdc, nx[j], ny[j] );
                                if ( nx[i] == nx[j] && ny[i] > ny[j] ) {
                                    arrow(90, nx[j], ny[j]+dy);
                                }
                                else if ( nx[i] == nx[j] && ny[i] < ny[j] ) {
                                    arrow(-90, nx[j], ny[j]-dy);
                                }
                                else if ( nx[i] > nx[j] ) {
                                     arrow(180, nx[j]+dx, ny[j]);
                                }
                                else {
                                    arrow(0, nx[j]-dx, ny[j]);
                                }
                            }

                        }
                    }
                }
            }

            for ( i = 0; i <= 10; i++ ){
            if ( !check ) SelectObject(hdc, finished);
            else SelectObject(hdc, BPen);
              Ellipse ( hdc, nx[i]-dx, ny[i]-dy, nx[i]+dx, ny[i]+dy );
              TextOut ( hdc, nx[i]-dtx, ny[i]-dy/2, nn[i], 1 );
            }
}

void printMatrix ( double** matrix, int rows, int cols ) {
    for ( int i = 0; i < rows; i++ ){
        for ( int j = 0; j < cols; j++ )
            printf("%.0f\t", matrix[i][j]);
        printf("\n");
    }
}


void generateMatrixes ( int k )
{
    for ( int i = 0; i < numb; i++ ) {
        if ( i == 0 ) {
            nx[i] = oneLen*3;
            ny[i] = oneLen;
        }
        else if ( i < 3) {
            nx[i] = nx[i-1] - oneLen;
            ny[i] = ny[i-1];
        }
        else if ( i < 5 ) {
            nx[i] = nx[i-1];
            ny[i] = ny[i-1] + oneLen;
        }
        else if ( i < 9 ){
            nx[i] = nx[i-1] + oneLen;
            ny[i] = ny[i-1];
        }
        else {
            nx[i] = nx[i-1];
            ny[i] = ny[i-1] - oneLen;
        }
    }
    srand(0412);
    double** T = randmm(numb, numb);
    double coef;
    if ( !k ) coef = 1.0 - 1*0.01 - 2*0.01 - 0.3;
    else coef = 1.0 - 1*0.005 - 2*0.005 - 0.27;
    mat = mulmr(coef, T, numb, numb);
    mirror = randmm(numb, numb);
    tree = randmm(numb, numb);
    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ )
            tree[i][j] = 0;
    printf("\nMatrix:\n");
    printMatrix ( mat, numb, numb );
    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ )
            mirror[i][j] = mat[i][j];
    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ )
            if ( mat[i][j] ) mirror[j][i] = mat[i][j];
    printf("\nMirror matrix:\n");
    printMatrix ( mirror, numb, numb );
}


void matMath (  )
{
    int Nsteps[numb] = {  };
    for ( int i = 0; i < numb; i++ ) Nsteps[i] = 0;
    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ )
            if ( mirror[i][j] ) Nsteps[i]++;
    printf("\nNot oriented steps:\n");
    for ( int i = 0; i < numb; i++ ) printf("%i\t", Nsteps[i]);
    int Osteps[numb][2] = {};
    for ( int i = 0; i < numb; i++ ) {
        Osteps[i][0] = 0;
        Osteps[i][1] = 0;
    }
    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ )
            if ( mat[i][j] ) {
                Osteps[i][1]++;
                Osteps[j][0]++;
            }
    printf("\nOriented steps:");
    printf("\nIN\tOUT\n");
    for ( int i = 0; i < numb; i++ ) {
            printf("%i\t%i", Osteps[i][0],Osteps[i][1]);
        printf("\n");
    }
    printf("\nIsolated and 1-way\'s:\n");
    for ( int i = 0; i < numb; i++ )
        if ( !Osteps[i][0] && !Osteps[i][1] ) printf("%i - isolated\n", i);
        else if ( (!Osteps[i][0] && Osteps[i][1] == 1) || (!Osteps[i][1] && Osteps[i][0] == 1) ) printf("%i - in air\n", i);
    printf("\nWays length 2:\n");
    double** T = randmm(numb, numb);
    double** ways = multip(mat, mat, T, numb, numb);
    for ( int i = 0; i < numb; i++ ) {
        for ( int j = 0; j < numb; j++ )
            if ( ways[i][j] && i != j ) printf("%i -> %i  ", i+1, j+1);
        printf("\n");
    }
    printf("\nWays length 3:\n");
    T = randmm(numb,numb);
    double** waysT = multip( ways, mat, T, numb, numb );
    for ( int i = 0; i < numb; i++ ) {
        for ( int j = 0; j < numb; j++ )
            if ( waysT[i][j] && i != j ) printf("%i -> %i  ", i+1, j+1);
        printf("\n");
    }
    printf("\nAccessability matrix:\n");
    double** accesM = randmm(numb,numb);
    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ )
            accesM[i][j] = mat[i][j];
    for ( int i = 0; i < numb - 1; i++ ) {
        for ( int k = 0; k < numb; k++ )
            for ( int f = 0; f < numb; f++ )
                if ( ways[k][f] || accesM[k][f] ) accesM[k][f] = 1;
                else accesM[k][f] = 0;
        T = randmm(numb,numb);
        ways = multip(ways, mat, T, numb, numb);
    }
    printMatrix(accesM, numb, numb);
    printf("\nStrong connection matrix:\n");
    T = randmm(numb,numb);
    double** tranAcces = transp(accesM, T, numb, numb);
    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ )
            if ( accesM[i][j] && tranAcces[i][j] ) T[i][j] = 1;
            else T[i][j] = 0;
    printMatrix(T, numb, numb);
    printf("\nStrong connection components:\n");
    int checkConn[numb] = {};
    for ( int i = 0; i < numb; i++ ) checkConn[i] = 0;
    for ( int i = 0; i < numb; i++ )
        if ( !checkConn[i] ) {
            printf("{ ");
            for ( int j = i; j < numb; j++ )
                if ( T[i][j] ) {
                    printf("%i ", j+1);
                    checkConn[j] = 1;
                }
            printf(" }\n");
        }
}


void checker ( HDC hdc )
{
    if ( !check ) drawGraph( hdc, mat );
    else drawGraph( hdc, tree );
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


DWORD WINAPI secondfun (LPVOID lpParam)                                                             //second
{
    generateMatrixes( 0 );
    matMath();
    printf("\nSwitch up formula\n");
    pausa( lpParam );
    generateMatrixes( 1 );
    matMath();

}


char ProgName[]="Lab 4";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG lpMsg;
    WNDCLASS w;
    w.lpszClassName=ProgName;
    w.hInstance=hInstance;
    w.lpfnWndProc=WndProc;
    w.hCursor=LoadCursor(NULL, IDC_ARROW);
    w.hIcon=0;
    w.lpszMenuName=0;
    w.hbrBackground = LTGRAY_BRUSH;
    w.style=CS_HREDRAW|CS_VREDRAW;
    w.cbClsExtra=0;
    w.cbWndExtra=0;
    if(!RegisterClass(&w))
        return 0;
    hWnd=CreateWindow(ProgName,
        "Lab 4",
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        1920,
        1080,
        (HWND)NULL,
        (HMENU)NULL,
        (HINSTANCE)hInstance,
        (HINSTANCE)NULL);
    ShowWindow(hWnd, nCmdShow);

    DWORD dwThreadId = 1;
    HANDLE secpot;
    secpot = CreateThread(NULL, 0, secondfun, &hWnd, 0, &dwThreadId);

    while(GetMessage(&lpMsg, hWnd, 0, 0)) {
            TranslateMessage(&lpMsg);
            DispatchMessage(&lpMsg);
        }
    return(lpMsg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT messg,
                        WPARAM wParam, LPARAM lParam)
    {
    HDC hdc;
    PAINTSTRUCT ps;
    switch(messg){
        case WM_PAINT :

            hdc=BeginPaint(hWnd, &ps);
            checker ( hdc );
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return(DefWindowProc(hWnd, messg, wParam, lParam));
    }
    return 0;
}
