import java.util.Scanner;
import java.io.File;
import java.util.Locale;
class Fila {
    Game inicio;
    Game ultimo;

    public Fila() {
        this.inicio = null;
        this.ultimo = null;
    }

    public void Inserir(Game x) {
        if (inicio == null) {
            inicio = x;
            ultimo = x;
        } else {
            ultimo.prox = x;
            ultimo = x;
        }
        x.prox=null;
    }
    public Game Remove() throws Exception {
        if (inicio == null) {
            throw new Exception("V");
        }

        Game tmp = inicio;
        inicio = inicio.prox;

        if (inicio == null) { // se esvaziou
            ultimo = null;
        }

        return tmp;
    }

    public void Mostrar() {
        int n = 0;
        for (Game i = inicio; i != null; i = i.prox, n++) {
            System.out.print("[" + n + "] ");
            i.print();
        }
    }
}


class Game{
    int AppId;
    String Nome;
    String ReleaseDate;
    int EstimedOwners;
    float preco;
    String[] SupportLang;
    int MetaScore;
    int UserScore;
    int achievements;
    String[] publisher;
    String[] Dev;
    String[] Categoria;
    String[] Genres;
    String[] tags;
    Game prox;
    public Game(int Id, String Nome, String ReleaseDate, int EstimedOwners, float preco, String[] SupportLang, int MetaScore, int UserScore, int achievements, String[] publisher, String[] Dev, String[] Categoria, String[] Genres, String[] tags){
        this.AppId=Id;
        this.Nome=Nome;
        this.ReleaseDate=ReleaseDate;
        this.EstimedOwners=EstimedOwners;
        this.preco=preco;
        this.SupportLang=SupportLang;
        this.MetaScore=MetaScore;
        this.UserScore=UserScore;
        this.achievements=achievements;
        this.publisher=publisher;
        this.Dev=Dev;
        this.Categoria=Categoria;
        this.Genres=Genres;
        this.tags=tags;
        this.prox=null;
    }
    public int getAchievements() {
        return achievements;
    }
    public int getAppId() {
        return AppId;
    }
    public String[] getCategoria() {
        return Categoria;
    }
    public String[] getDev() {
        return Dev;
    }
    public int getEstimedOwners() {
        return EstimedOwners;
    }
    public String[] getGenres() {
        return Genres;
    }
    public int getMetaScore() {
        return MetaScore;
    }
    public String getNome() {
        return Nome;
    }
    public float getPreco() {
        return preco;
    }
    public String[] getPublisher() {
        return publisher;
    }
    public String getReleaseDate() {
        return ReleaseDate;
    }
    public String[] getSupportLang() {
        return SupportLang;
    }
    public String[] getTags() {
        return tags;
    }
    public int getUserScore() {
        return UserScore;
    }
    public void setAchievements(int achievements) {
        this.achievements = achievements;
    }
    public void setAppId(int appId) {
        AppId = appId;
    }
    public void setCategoria(String[] categoria) {
        Categoria = categoria;
    }
    public void setDev(String[] dev) {
        Dev = dev;
    }
    public void setEstimedOwners(int estimedOwners) {
        EstimedOwners = estimedOwners;
    }
    public void setGenres(String[] genres) {
        Genres = genres;
    }
    public void setMetaScore(int metaScore) {
        MetaScore = metaScore;
    }
    public void setNome(String nome) {
        Nome = nome;
    }
    public void setPreco(float preco) {
        this.preco = preco;
    }
    public void setPublisher(String[] publisher) {
        this.publisher = publisher;
    }
    public void setReleaseDate(String releaseDate) {
        ReleaseDate = releaseDate;
    }
    public void setSupportLang(String[] supportLang) {
        SupportLang = supportLang;
    }
    public void setTags(String[] tags) {
        this.tags = tags;
    }
    public void setUserScore(int userScore) {
        UserScore = userScore;
    }
    public void print(){
        System.out.print("=> "+AppId + " ## " +Nome + " ## " +ReleaseDate + " ## " + EstimedOwners + " ## " +preco + " ## [");
        for(int i=0;SupportLang[i]!=null;i++){
            if(SupportLang[i+1]==null){
                System.out.print(SupportLang[i]);
            }
            else{
                System.out.print(SupportLang[i] + ", ");
            }
        }
        System.out.print("] ## "+MetaScore + " ## ");
        if(UserScore==0){
            System.out.print(UserScore+".0");
        }
        else{
            System.out.print(UserScore);
        }
        System.out.print(" ## " + achievements + " ## [");
        for(int i=0;i<publisher.length && publisher[i]!=null;i++){
            if(i==publisher.length-1 || publisher[i+1]==null){
                System.out.print(publisher[i]);
            }
            else{
                System.out.print(publisher[i] + ", ");// Nome de classe maiusculo, variavel minusculo
            }
        }
        System.out.print("] ## [");
        for(int i=0;i<Dev.length && Dev[i]!=null;i++){
            if(i==Dev.length-1 || Dev[i+1]==null){
                System.out.print(Dev[i]);
            }
            else{
                System.out.print(Dev[i] + ", ");
            }
        }
        System.out.print("] ## [");
        for(int i=0;i<Categoria.length && Categoria[i]!=null;i++){
            if(i==Categoria.length-1 || Categoria[i+1]==null){
                System.out.print(Categoria[i]);
            }
            else{
                System.out.print(Categoria[i] + ", ");
            }
        }
        System.out.print("] ## [");
        for(int i=0;i<Genres.length && Genres[i]!=null;i++){
            System.out.print(Genres[i]);
            if(Genres[i+1]!=null){
                System.out.print(", ");
            }
        }
        System.out.print("] ## [");
        for(int i=0;tags[i]!=null && tags[i]!=null;i++){
            System.out.print(tags[i]);
            if(tags[i+1]!=null){
                System.out.print(", ");
            }
        }
        System.out.println("] ##");
    }
    
}   
class Data{
    int dia;
    int mes;
    int ano;
    String resp;
    public Data(){
        this.dia=dia;
        this.mes=mes;
        this.ano=ano;
        this.resp="";
    }
    public String setData(){
        if(dia<10){
            resp+="0"+dia+"/";
        }
        else{
            resp+=dia+"/";
        }
        if(mes<10){
            resp+="0"+mes+"/";
        }
        else{
            resp+=mes+"/";
        }
        resp+=ano;
        return resp;
    }
    public int SetMes(String Data){
        Scanner sf=new Scanner(Data);
        String Mes=sf.next();
        if(Mes.equals("Jan")){
            mes=01;
        }
        else if(Mes.equals("Feb")){
            mes=02;
        }
        else if(Mes.equals("Mar")){
            mes=03;
        }
        else if(Mes.equals("Apr")){
            mes=04;
        }
        else if(Mes.equals("May")){
            mes=05;
        }
        else if(Mes.equals("Jun")){
            mes=06;
        }
        else if(Mes.equals("Jul")){
            mes=07;
        }
        else if(Mes.equals("Aug")){
            mes=8;
        }
        else if(Mes.equals("Sep")){
            mes=9;
        }
        else if(Mes.equals("Oct")){
            mes=10;
        }
        else if(Mes.equals("Nov")){
            mes=11;
        }
        else if(Mes.equals("Dec")){
            mes=12;
        }
        sf.close();
        return mes;
    }
    public int SetDiaAno(String Data) {
        Scanner sf = new Scanner(Data);
        sf.useDelimiter("[ ,]+"); // separa por espaço ou vírgula

        sf.next(); // pula o mês

        String aux = sf.next(); // pode ser o dia ou o ano
        int token = 0;
        for (int i = 0; i < aux.length(); i++) {
            char c = aux.charAt(i);
            if (c >= '0' && c <= '9') {
                token = token * 10 + (c - '0');
            }
        }

        if (token <= 31) {
            dia = token;
            ano = sf.nextInt();
        } else {
            dia = 1;
            ano = token;
        }

        sf.close();
        return ano;
    }
}
class Q4 {
    public static void quicksort(int esq, int dir, Game []array) {
        int i = esq, j = dir;
        Game pivo = array[(esq+dir)/2];
        while (i <= j) {
            while (array[i].AppId < pivo.AppId){
                i++;
            }
            while (array[j].AppId > pivo.AppId){
                j--;
            }
            if (i <= j){
                Game tmp= array[i];
                array[i]=array[j];
                array[j]=tmp;
                i++;
                j--;
            }
        }
        if (esq < j){
            quicksort(esq, j, array);
        }
        if (i < dir){
            quicksort(i, dir, array);
        }

    }
    public static Game PesquisaBin(int esq, int dir, int x, Game[] array) {
        Game tmp = null;
        if (esq <= dir) {
            int pivo = (esq + dir) / 2;

            if (array[pivo].AppId < x) {
                tmp = PesquisaBin(pivo + 1, dir, x, array);
            } 
            else if (array[pivo].AppId > x) {
                tmp = PesquisaBin(esq, pivo - 1, x, array);
            } 
            else if (array[pivo].AppId == x) {
                tmp = array[pivo];
            }
        }
        return tmp;
    }

    public static void main(String[] args) throws Exception {
        File csv = new File("//tmp//games.csv");
        Scanner sc = new Scanner(csv); 
        sc.useLocale(Locale.US);
        sc.nextLine();
        Game[] array=new Game[1850];
        int l=0;
        while(sc.hasNext()){
            sc.useDelimiter("[,\n]");   
            int Id = sc.nextInt();
            String token = sc.next();
            String Nome = "";
            if(token.charAt(0)=='"'){
                sc.useDelimiter("\"");
                Nome=""+token+sc.next();
                sc.useDelimiter("[,\n]");
                sc.next();
            }
            else{
                Nome=token;
            }
            token=sc.next();
            String ReleaseDate;
            if(token.charAt(0)=='"'){
                sc.useDelimiter("\"");
                ReleaseDate=""+token+sc.next();
                String corrigido="";
                for(int i=1;i<ReleaseDate.length();i++){
                    corrigido+=""+ReleaseDate.charAt(i);
                }
                ReleaseDate=corrigido;
                sc.useDelimiter("[,\n]");
                sc.next();
            }
            else{
                ReleaseDate=token;
            }
            Data a=new Data();
            a.SetMes(ReleaseDate);
            a.SetDiaAno(ReleaseDate);
            a.setData();

            int EstimedOwners = sc.nextInt();
            float preco = sc.nextFloat();
            //
            
            String[] supportLang = new String[100];
            sc.useDelimiter("[,\n]");
            token = sc.next();
            if(token.charAt(0)=='"'){
                sc.useDelimiter("\"");
                String publish=sc.next(); // "['English[a]','English[b]']",
                String corrigido="";
                for(int i=3;i<token.length()-1;i++){
                    corrigido=corrigido+""+token.charAt(i);
                }
                supportLang[0]=corrigido;
                corrigido="";
                for(int i=0;i<publish.length()-1;i++){
                    corrigido=corrigido+""+publish.charAt(i);
                }
                publish=corrigido;
                Scanner lang=new Scanner(publish);
                lang.useDelimiter("'");
                for(int i=1;lang.hasNext();i++){
                    lang.next();
                    supportLang[i]=lang.next();
                }
                lang.close();
                sc.useDelimiter("[,\n]");
                sc.next();
            }
            else{
                String corrigido="";
                for(int i=2;i<token.length()-2;i++){
                    corrigido=corrigido+""+token.charAt(i);
                }
                supportLang[0]=corrigido;
            }
            //
            int MetaScore = sc.nextInt();
            int UserScore = sc.nextInt();
            int achievements = sc.nextInt();

            // -------- Publisher --------
            String[] Publisher = new String[50];
            String publish = "";
            token = sc.next();
            if (token.length() > 0 && token.charAt(0) == '"') {
                sc.useDelimiter("\"");
                publish = sc.next();
                sc.useDelimiter("[,\n]");
                Publisher[0]=token;
                sc.next();
            } else {
                Publisher[0] = token;
            }
            Scanner pub = new Scanner(publish);
            pub.useDelimiter(",");
            for (int i=1; pub.hasNext(); i++){
                Publisher[i] = pub.next();
            }
            pub.close();

            // -------- Dev --------
            String[] Dev = new String[50];
            String devStr = "";
            token = sc.next();
            if (token.length() > 0 && token.charAt(0) == '"') {
                sc.useDelimiter("\"");
                devStr = sc.next();
                String Corrigido="";
                for(int i=1;i<token.length();i++){
                    Corrigido+=token.charAt(i);
                }
                Dev[0]=Corrigido;
                sc.useDelimiter("[,\n]");
                sc.next();
            } else {
                Dev[0] = token;
            }
            Scanner devScan = new Scanner(devStr);
            devScan.useDelimiter(",");
            for (int i=1; devScan.hasNext(); i++){
                Dev[i] = devScan.next();
            }
            devScan.close();

            // -------- Categoria --------
            String[] Categoria = new String[100];
            String categoriaStr = "";
            token = sc.next();
            if (token.length() > 0 && token.charAt(0) == '"') {
                sc.useDelimiter("\"");
                categoriaStr = sc.next();
                sc.useDelimiter("[,\n]");
                sc.next();
                String Corrigido="";
                for(int i=1;i<token.length();i++){
                    Corrigido+=token.charAt(i);
                }
                Categoria[0]=Corrigido;
            } else {
                Categoria[0]= token;
            }
            Scanner catScan = new Scanner(categoriaStr);
            catScan.useDelimiter(",");
            for (int i=1; catScan.hasNext(); i++){
                Categoria[i] = catScan.next();
            }
            catScan.close();

            // -------- Genres --------
            String[] genries = new String[100];
            String genStr = "";
            token = sc.next();
            if (token.length() > 0 && token.charAt(0) == '"') {
                sc.useDelimiter("\"");
                genStr = sc.next();
                sc.useDelimiter("[,\n]");
                sc.next();
                String corrigido="";
                for(int i=1;i<token.length();i++){
                    corrigido+=token.charAt(i);
                }
                genries[0]=corrigido;
            } else {
                genries[0]=token;
            }
            Scanner genScan = new Scanner(genStr);
            genScan.useDelimiter(",");
            for (int i=1; genScan.hasNext(); i++){
                genries[i] = genScan.next();
            }
            genScan.close();

            // -------- Tags --------
            String[] tags = new String[100];
            String tagStr = "";
            token = sc.next();
            if (token.length() > 0 && token.charAt(0) == '"') {
                sc.useDelimiter("\"");
                tagStr = sc.next();
                sc.useDelimiter("[,\n]");
                String Corrigido="";
                for(int i=1;i<token.length();i++){
                    Corrigido+=token.charAt(i);
                }
                tags[0]=Corrigido;
                sc.next();
            } else {
                tagStr = token;
            }
            Scanner tagScan = new Scanner(tagStr);
            tagScan.useDelimiter(",");
            for (int i=1; tagScan.hasNext(); i++){
                tags[i] = tagScan.next();
            }
            tagScan.close();

            array[l] = new Game(Id, Nome, a.resp, EstimedOwners, preco, supportLang, MetaScore, UserScore, achievements, Publisher, Dev, Categoria, genries, tags);
            l++;
        }
        sc.close();
        quicksort(0,array.length-1,array);
        //Comandos
        Scanner esc=new Scanner(System.in);
        Fila a=new Fila();
        while(esc.hasNextInt()){
            a.Inserir(PesquisaBin(0, array.length-1, esc.nextInt(), array));
        }
        esc.next();//come fim
        int d=esc.nextInt();
        for(;d>0;d--){
            String c=esc.next();
            if(c.equals("I")){
                a.Inserir(PesquisaBin(0, array.length-1, esc.nextInt(), array));
            }
            
            else if(c.equals("R")){
                System.out.println("(R) "+a.Remove().Nome);
            }
        }
        
        a.Mostrar();
        esc.close();
    }
}
