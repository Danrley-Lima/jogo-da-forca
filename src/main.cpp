#include "Forca.hpp"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    Forca forca(argv[1], argv[2]); // Construindo a classe "forca".
    auto valid = forca.eh_valido(); // Verificação de todos os testes.

    // Primeira exibição de Interface.
    cout << ">>> Lendo arquivos de palavras ["<< argv[1] <<"] e scores ["<< argv[2] <<"], por favor aguarde.." << endl;
    cout << "--------------------------------------------------------------------" << endl;
    if (!valid.first) { cout<< valid.second <<endl; exit(-1); }
    else { cout << "Arquivos OK!" << endl; }
    cout << "--------------------------------------------------------------------" << endl;

    forca.carregar_arquivos();
    
    int opcao, dificuldade, tentativas = 6, acertos = 1, dificuldade_escolhida = 0, pontuacao = 0;
    char palpite, vogal;
    vector<char> consoante;
    bool aux_tentativas;
    string palavra_secreta, nome, palavra, dificuldade_scores;
    vector<char> letras_erradas;
    vector<string> palavras_acertadas_inf;


    // Loop principal do jogo.
    while(true) {
        cout << "Bem vindo ao Jogo Forca! Por favor escolha uma das opções" << endl;
        cout << "1 - Iniciar Jogo" << endl;
        cout << "2 - Ver scores anteriores" << endl;
        cout << "3 - Sair do Jogo" << endl;
        cout << "Sua escolha: ";
        cin >> opcao; // Ler opção do usuário.
        cout << endl;

        tela_inicial:
        // Caso a escolha do jogador perante a interface seja '1' :
        if (opcao == 1) {
            cout << "Vamos iniciar o jogo! Por favor escolha o nível de dificuldade" << endl;
            cout << "1 - Fácil" << endl;
            cout << "2 - Médio" << endl;
            cout << "3 - Difícil" << endl;
            cout << "Sua escolha: ";
            cin >> dificuldade; // Ler dificuldade escolhida.
            cout << endl;

            if (dificuldade == 1){
                dificuldade_scores = "FACIL";
                dificuldade_escolhida = 0;
                forca.set_dificuldade(dificuldade_escolhida);
                palavra = forca.sorteia_palavra(forca.separar_por_dificuldade());
                palavra_secreta = forca.get_palavra_atual(palavra);
                consoante = forca.muda_valor_consoante_mapa(palavra_secreta, dificuldade_escolhida);
                cout << "Iniciando o Jogo no nível fácil, será que você conhece essa palavra?" << endl;
            } else if (dificuldade == 2){
                dificuldade_scores = "MEDIO";
                dificuldade_escolhida = 1; 
                forca.set_dificuldade(dificuldade_escolhida);
                palavra = forca.sorteia_palavra(forca.separar_por_dificuldade());
                palavra_secreta = forca.get_palavra_atual(palavra);
                vogal = forca.muda_valor_vogal_mapa(palavra_secreta, dificuldade_escolhida);
                cout << "Iniciando o Jogo no nível médio, será que você conhece essa palavra?" << endl;
            } else if (dificuldade == 3){
                dificuldade_scores = "DIFICIL";
                dificuldade_escolhida = 2;
                forca.set_dificuldade(dificuldade_escolhida);
                palavra = forca.sorteia_palavra(forca.separar_por_dificuldade());
                palavra_secreta = forca.get_palavra_atual(palavra);
                cout << "Iniciando o Jogo no nível difícil, será que você conhece essa palavra?" << endl;
            }
            cout << "Palavra secreta: " << palavra_secreta << endl;
            while(true) {
                lde:
                while(true) { //Loop da Rodada.
                    string jogada_informacoes;
                    cout << endl;
                    forca.imprimir_underline(palavra_secreta, consoante, vogal, dificuldade_escolhida);
                    cout << endl;
                    forca.imprimir_chutes_errados();
                    cout << endl;
                    cout << "Pontos: " << pontuacao << " | Tentativas: " << tentativas << endl;
                    cout << "Palpite: ";
                    cin >> palpite; //Ler o palpite.
                    cout << endl;
                    palpite = toupper(palpite); //Modifica a leitura pra maiúsculo.

                    //Marca true no map das letras da palavra.
                    forca.muda_valor_letra_mapa(palpite);

                    //Calcula tentativas.
                    aux_tentativas = forca.verifica_tentativas(palavra_secreta, palpite, tentativas);
                    if(aux_tentativas){ tentativas--;}
                    if (*find(palavra_secreta.begin(), palavra_secreta.end(), palpite) == palpite){ tentativas++; }

                    //Verifica se letra chutada existe na palavra.
                    bool resultado = forca.letra_existe(palpite, palavra_secreta);

                    letras_erradas = forca.vector_letra_errada(palavra_secreta, palpite);

                    if (resultado) {
                        if (dificuldade_escolhida == 0){
                            for (int i = 0; i < (int)consoante.size(); i++){
                                if (palpite == consoante[i]){
                                    cout << "Você já chutou essa letra!" << endl;
                                    goto lde;
                                }
                            }
                        } else if (dificuldade_escolhida == 1){
                            if (palpite == vogal){
                                cout << "Você já chutou essa letra!" << endl;
                                goto lde;
                            }
                        }
                        cout << "--------------------------------------------------------------------" << endl;
                        cout << "Muito bem! A palavra contém a letra "<< palpite << "!" << endl;
                        cout << "--------------------------------------------------------------------" << endl;
                        forca.get_letras(palpite, consoante, vogal, resultado, acertos, pontuacao);
                        forca.pontuacao_jogador(palavra_secreta, palpite, pontuacao, resultado);
                    } else {
                        cout << "--------------------------------------------------------------------" << endl;
                        cout << "Meh, não achei a letra " << palpite << "! :<" << endl;
                        cout << "--------------------------------------------------------------------" << endl;
                        forca.get_letras(palpite, consoante, vogal, resultado, acertos, pontuacao);
                        forca.pontuacao_jogador(palavra_secreta, palpite, pontuacao, resultado);
                        forca.get_letra_erradas(palpite, palavra_secreta);
                        
                    }
                    forca.imprimir_boneco(forca.get_tam_letras_erradas());

                    //Se as tentativas acabarem ou enforcar o boneco, FIM DE JOGO.
                    if (tentativas == 0 || forca.verifica_derrota()){
                        char resposta;
                        cout << endl;
                        cout << "Suas tentativas acabaram ou você enforcou o boneco! Palavra secreta: " + palavra_secreta + "." << endl;
                        cout << endl;
                        cout << "Deseja jogar novamente?[S/N] ";
                        cin >> resposta;
                        resposta = (toupper(resposta));
                        switch (resposta){
                            case 'S':
                                forca.reset_rodada(tentativas, acertos, consoante, vogal, palavra_secreta, letras_erradas);
                                goto tela_inicial;
                                break;
                            case 'N':
                                cout << "Digite o seu nome: ";
                                cin >> nome;
                                transform(nome.begin(), nome.end(),nome.begin(), ::toupper);
                                cout << endl;
                                cout << "--------------------------------------------------------------------" << endl;
                                cout << "----------- Salvando informações no arquivo dos scores. ------------" << endl;
                                cout << "--------------------------------------------------------------------" << endl;
                                //Guardar informações.
                                jogada_informacoes = '\n' + dificuldade_scores + ';' + nome  + ';';
                                forca.escrever_scores(jogada_informacoes, palavras_acertadas_inf, pontuacao);
                                exit(-1);
                            
                            default:
                                cout << "Caractere inválido." << endl;
                                break;
                        }
                        cout << endl;
                    }

                    //Se descobrir todas as letras da palavra, VITÓRIA.
                    if (forca.verifica_vitoria(palpite, palavra_secreta, acertos, dificuldade_escolhida)){
                        char resposta;
                        cout << "Parabéns! Você conseguiu descobrir a palavra secreta: " + palavra_secreta + "." << endl;
                        cout << endl;
                        cout << "Deseja jogar novamente?[S/N] ";
                        cin >> resposta;
                        resposta = (toupper(resposta));
                        palavras_acertadas_inf.push_back(palavra_secreta + ',');
                        switch (resposta){
                            case 'S':
                                forca.reset_rodada(tentativas, acertos, consoante, vogal, palavra_secreta, letras_erradas);
                                fflush(stdin);
                                goto tela_inicial;
                                break;
                            case 'N':
                                cout << "Digite o seu nome: ";
                                cin >> nome;
                                transform(nome.begin(), nome.end(),nome.begin(), ::toupper);
                                cout << endl;
                                cout << "--------------------------------------------------------------------" << endl;
                                cout << "---------- Salvando informações no arquivo dos scores. -----------" << endl;
                                cout << "--------------------------------------------------------------------" << endl;
                                //Guardar informações.
                                jogada_informacoes = '\n' + dificuldade_scores + ';' + nome  + ';';
                                forca.escrever_scores(jogada_informacoes, palavras_acertadas_inf, pontuacao);
                                exit(-1);
                            default:
                                cout << "Caractere inválido." << endl;
                                break;
                        }
                        cout << endl;
                    }
                }
            }
            // Ler informações do jogador para o score e gravar no arquivo.
        // Caso a escolha do jogador perante a interface seja '1' :
        } else if (opcao == 2) {
            forca.mostrar_scores();
        } else { // Caso a opção seja 3 para sair do jogo ou alguma outra não especificada, o jogo encerra.
            break;
        }    
    }
    return 0;
}