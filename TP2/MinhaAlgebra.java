import java.util.Scanner;

public class MinhaAlgebra {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int qtdVar = sc.nextInt();

        while (qtdVar != 0) {
            int[] valores = new int[qtdVar];
            for (int i = 0; i < qtdVar; i++) {
                valores[i] = sc.nextInt();
            }

            String expressao = sc.nextLine().trim();
            int resultado = avalia(expressao, valores);
            System.out.println(resultado);

            qtdVar = sc.nextInt();
        }

        sc.close();
    }

    public static int avalia(String expr, int[] valores) {
        expr = expr.trim();

        if (expr.length() == 1 && expr.charAt(0) >= 'A' && expr.charAt(0) <= 'Z') {
            return valores[expr.charAt(0) - 'A'];
        }

        if (expr.startsWith("not")) {
            String dentro = pegaDentro(expr, 3);
            int val = avalia(dentro, valores);
            return (val == 0) ? 1 : 0;
        }

        if (expr.startsWith("and")) {
            String dentro = pegaDentro(expr, 3);
            String[] partes = separaArgs(dentro);
            int v1 = avalia(partes[0], valores);
            int v2 = avalia(partes[1], valores);
            return (v1 == 1 && v2 == 1) ? 1 : 0;
        }

        if (expr.startsWith("or")) {
            String dentro = pegaDentro(expr, 2);
            String[] partes = separaArgs(dentro);
            int v1 = avalia(partes[0], valores);
            int v2 = avalia(partes[1], valores);
            return (v1 == 1 || v2 == 1) ? 1 : 0;
        }

        return 0;
    }

    private static String pegaDentro(String expr, int tamOperador) {
        int ini = expr.indexOf('(', tamOperador) + 1;
        int fim = expr.lastIndexOf(')');
        return expr.substring(ini, fim).trim();
    }

    private static String[] separaArgs(String dentro) {
        int nivel = 0;
        int corte = -1;
        for (int i = 0; i < dentro.length(); i++) {
            char c = dentro.charAt(i);
            if (c == '(') nivel++;
            else if (c == ')') nivel--;
            else if (c == ',' && nivel == 0) {
                corte = i;
                break;
            }
        }
        String arg1 = dentro.substring(0, corte).trim();
        String arg2 = dentro.substring(corte + 1).trim();
        return new String[]{arg1, arg2};
    }
}
