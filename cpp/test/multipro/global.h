#ifndef GLOBALH
#include <string>
#include <map>

using namespace std;

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--)

class Config {
    map<string,string> tabla;
    public:
    Config(const char *fname);
    string getVal(string key);
    static Config* getInstance();    
};

Config::Config(const char *fname) {
    FILE *f = fopen(fname, "r");
    char act;
    char buf[256];
    do {
        act = fgetc(f);
        if (act == '#') {
            while (fgetc(f) != '\n');
        } else if (act!='\n' && act!=' ' && act!='\t' && act!=EOF) {
            int i=0;
            do {
                buf[i++]=act;
            } while ((act=fgetc(f))!='=' && act!=EOF);
            buf[i]='\0';
            string key(buf);

            i=0;
            while ((act=fgetc(f))!='\n' && act!=EOF) {
                buf[i++]=act;
            }
            buf[i]='\0';
            string value(buf);
            tabla[key] = value;
        }

    } while (act != EOF);
}

string Config::getVal(string key) {
    return tabla[key];
}

void printMat(Mat *x) {
    rep(i,x->nrows()) {
	    rep(j,x->ncols())
	        printf("%.2lf\t",x->get(i,j));
	    printf("\n");
    }
    printf("\n");
}

#endif
