#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include "texas.h"
#include "basic.h"


extern const int SCREEN_WIDTH = 1080;
extern const int SCREEN_HEIGHT = 720;

//Button constants
extern const int BUTTON_WIDTH = 200;
extern const int BUTTON_HEIGHT = 120;

extern const int NUM_BUT_L = SCREEN_WIDTH/16;
extern const int blind_size = 70;

LTexture hand1texture;
LTexture hand2texture;
LTexture generaltexture;
LTexture gBackgroundTexture;
LButton gbottum[5];
LButton exit_bottum;
LButton resume_bottum;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;



Uint32 timer_callback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;
    userevent.type = SDL_USEREVENT;
    userevent.code = 1032;
    userevent.data1 = NULL;
    userevent.data2 = NULL;
    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);

    return(interval);
}

class game
{
private:
    int windows_height,windows_width;
    double time;

    SDL_Window* main_window;
    SDL_Surface* main_surface;
    SDL_Renderer* main_renderer;

    int ptr = -1;//direct window

    int cardlist[4][13];// save player's cards
    int cardlistcount[4];//card numbers of each player
    int now_player = 0;
    std::vector<int> middle;//four cards on display

    int selected_card = -1;
    double selected_rmv;//card right shift how much

    SDL_Texture** card_images;
    std::string* card_paths;//pictures/
    unsigned card_count;
    int player_win[4];//0 2  1 3 need how many to win
    int org_king;//no king_type on display the first is king_type
    bool win;//check if any team win
    int king_type = -1;
    int win_cond[4];//win condition
    int button_count;//choose king card need to display how many card
    int button_select=-1;
    int selec[5] = {56,57,58,59,60};//initial skip club1 diamond1 heart1 spade1
    int selec_stack=0;//time of skip
    bool decided = false;//decide king_type
    bool able_decided = false;//if four players skip go back to first

public:
    game()
    {
        for(int i=0;i<4;i++)
        {
            cardlistcount[i] = 0;
        }
    }

    int max;//winner of each round
    int playercard_number[4][13];
    int playercard_color[4][13];

    void init(const char* title)
    {
        SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER );
        IMG_Init( IMG_INIT_PNG );
        main_window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windows_width, windows_height, SDL_WINDOW_SHOWN );
        main_surface = SDL_GetWindowSurface(main_window);
        main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
    }

    void close()
    {
        SDL_DestroyWindow( main_window );
        main_window = NULL;
        SDL_Quit();
    }

    void load_card()
    {
        card_images = new SDL_Texture*[card_count];
        for(int i=0;i<card_count;i++)
        {
            card_images[i] = SDL_CreateTextureFromSurface(main_renderer,IMG_Load( card_paths[i].c_str() ));
        }
    }

    void print_card_set(int* cardlisto,int cardc,double ystr)
    {
        double rmove = 0.6/cardc;
        for(int i=0;i<cardc;i++)
        {
            present_image(card_images[cardlisto[i]],0.2+rmove*i,ystr,0.1,0.2);
        }
    }

    void print_card_set_mid(int* cardlisto,int cardc,double ystr)
    {
        double rmove = 0.4/cardc;
        for(int i=0;i<cardc;i++)
        {
            present_image(card_images[cardlisto[i]],0.3+rmove*i,ystr,0.1,0.2);
        }
    }

    void print_back_set_hor(int cardc,double ystr)
    {
        double rmove = 0.6/cardc;
        for(int i=0;i<cardc;i++)
        {
            present_image(card_images[52],0.2+rmove*i,ystr,0.1,0.2);
        }
    }

    void print_back_set_ver(int cardc,double xstr)
    {
        double rmove = 0.6/cardc;
        for(int i=0;i<cardc;i++)
        {
            present_image(card_images[55],xstr,0.15+rmove*i,(0.2*windows_height)/windows_width,(0.1*windows_width)/windows_height);
        }
    }

    void servecard()
    {
        bool cardserved[52]={false};
        for(int i=0; i<4; i++)
        {
            cardlistcount[i] = 13;
            for(int j=0; j<13; j++)
            {
                int key = rand()%52;
                while(cardserved[key])
                {
                    key = rand()%52;
                }
                cardserved[key]=true;
                cardlist[i][j] = key;
            }
        }
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<13; j++)
            {
                for(int k=0; k<j; k++)
                {
                    if((cardlist[i][j]%4)<cardlist[i][k]%4)
                    {
                        int temp=cardlist[i][k];
                        cardlist[i][k]=cardlist[i][j];
                        cardlist[i][j]=temp;
                    }
                }
            }
        }
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<13; j++)
            {
                for(int k=0; k<j; k++)
                {
                    if(cardlist[i][j]%4==cardlist[i][k]%4)
                    {
                        if(((cardlist[i][j]/4)%13) < (cardlist[i][k]/4)%13)
                        {
                            int temp=cardlist[i][k];
                            cardlist[i][k]=cardlist[i][j];
                            cardlist[i][j]=temp;
                        }
                    }
                }
            }
        }
    }

    void print_middle()
    {
        double ystr=0.35;
        int uls[10],cnt=0;
        for(int i=0;i<middle.size();i++)
        {
            uls[i%10] = middle[i];
            if(cnt == 10)
            {
                print_card_set_mid(uls,10,ystr);
                cnt = 0;
                ystr += 0.05;
            }
            cnt++;
        }
        print_card_set_mid(uls,cnt,ystr);
    }

    void remove_card(int id,int player)
    {
        int ptr=0;
        bool have_king = false;
        if(middle.size() != 0)
        {
            for(int i=0;i<cardlistcount[player];i++)
            {
                if(cardlist[player][i]%4==middle[0]%4)
                {
                    have_king=true;
                }
            }
        }

        bool exist = false;
        for(int i=0;i<cardlistcount[player];i++)
        {
            if(cardlist[player][i] == id)
            {
                if(have_king)
                {
                    if(cardlist[player][i]%4 == middle[0]%4)
                    {
                        middle.push_back(cardlist[player][i]);
                        exist = true;
                    }
                    else
                    {
                        cardlist[player][ptr] = cardlist[player][i];
                        ptr++;
                    }
                }
                else
                {
                    middle.push_back(cardlist[player][i]);
                    exist = true;
                }
            }
            else
            {
                cardlist[player][ptr] = cardlist[player][i];
                ptr++;
            }
        }

        if(exist)
        {
            cardlistcount[player]--;
        }
        else
        {
            now_player -= 1;
        }
    }

    void check_button(int x,int y)
    {
        printf("mouse down at (%d,%d) at %0.1lf sec. \n",x,y,time);
        printf("player %d select %d\n",now_player,selected_card);
        present_image(card_images[94+now_player], 0, 0, 0.02, 0.015);
        if(selected_card != -1)
        {
            remove_card(selected_card,now_player);
            now_player = (now_player+1)%4;
        }
        selected_card = -1;

        if(middle.size() == 4)
        {
            int cards[4];
            for(int i=0;i<4;i++)
            {
                cards[i] = middle[i];
            }
            middle.clear();

            find_king(cards);

            player_win[(now_player+max)  %4]++;
            player_win[(now_player+max+2)%4]++;
            for(int i=0;i<4;i++)
            {
                printf("player %d need %d round.\n",i,win_cond[i]-player_win[i]);
                if(player_win[i] >= win_cond[i])
                {
                    printf("player %d win.\n",i);
                    win=true;
                }
            }
            now_player = (now_player+max)  %4;
        }
    }

    void motion_button(int x,int y)
    {
        double rmove = 0.6/cardlistcount[now_player];
        selected_card = -1;
        for(int i=0;i<cardlistcount[now_player];i++)
        {
            if(x >= partital(0.2+i*rmove,windows_width) && x <= partital(0.2+(i+1)*rmove,windows_width) && y >= partital(0.7,windows_height) && y <= partital(0.9,windows_height))
            {
                selected_card = cardlist[now_player][i];
                selected_rmv = i*rmove;
            }
        }
    }

    SDL_Rect* get_rect(int x,int y,int w,int h)
    {
        SDL_Rect* rec = new SDL_Rect;
        rec->h = h;
        rec->w = w;
        rec->x = x;
        rec->y = y;
        return rec;
    }

    int partital(double rate,int cons)
    {
        return (int)(rate*cons);
    }

    void present_image(SDL_Texture* sdt,double start_wid_rate,double start_hei_rate,double end_wid_rate,double end_hei_rate)
    {
        SDL_RenderCopy(main_renderer,sdt,NULL,get_rect(partital(start_wid_rate,windows_width),partital(start_hei_rate,windows_height),partital(end_wid_rate,windows_width),partital(end_hei_rate,windows_height)));
    }

    void print_trump(int button_selecto)
    {
        present_image(card_images[57+28+5-1],0.7,0,0.2,0.1);
        if(button_selecto != -1)
        {
            present_image(card_images[57+28+button_selecto],0.9,0,0.1,0.1);
        }
    }


    void find_king(int cards[4])
    {
        bool is_king[4]={0};
        int card_color[4],card_number[4];
        std::cout<<"king type = "<<king_type<<std::endl;
        for(int i=0;i<4;i++)
        {
            card_color[i]=(cards[i])%4;//0 club 1 diamond 2 heart 3 spade
            card_number[i]=(cards[i]/4);
            std::cout<<"color : "<<card_color[i]<<" "<<card_number[i]<<std::endl;
        }

        for(int i=0; i<4; i++)
        {
            if(card_color[i]==king_type)
            {
                is_king[i]=true;
            }
        }

        if(is_king[0]&&is_king[1]&&is_king[2]&&is_king[3])
        {
            compare(card_number);
        }

        else if(is_king[0]||is_king[1]||is_king[2]||is_king[3])
        {
            for(int i=0; i<4; i++)
            {
                if(!is_king[i])
                {
                    card_number[i]=-1;
                }
            }
            compare(card_number);
        }

        else
        {
            int king=card_color[0];
            for(int i=0; i<4; i++)
            {
                if(card_color[i]==king)
                {
                    is_king[i]=true;
                }
            }
            for(int i=0; i<4; i++)
            {
                if(!is_king[i])
                {
                    card_number[i]=-1;
                }
            }
            compare(card_number);
        }
    }

    void compare(int card_number[4])
    {
        max=0;
        for(int i=0; i<4; i++)
        {
            if(card_number[i]>card_number[max])
            {
                max=i;
            }
        }
        std::cout<<"winner: "<<(now_player+max)  %4<<std::endl;
    }


    bool main_loop()
    {
        bool quit = false;
        for(int i=0;i<4;i++)
        {
            player_win[i] = 0;
        }

        win=false;

        while(!quit)
        {
            SDL_Event evt;
            present_image(card_images[54], 0, 0, 1, 1);
            while(SDL_PollEvent(&evt) != 0)
            {
                switch(evt.type)
                {
                case(SDL_QUIT):
                    quit = true;
                    break;
                case(SDL_MOUSEBUTTONDOWN):
                    check_button(evt.motion.x,evt.motion.y);
                    break;
                case(SDL_MOUSEMOTION):
                    motion_button(evt.motion.x,evt.motion.y);
                    break;
                case(SDL_USEREVENT):
                    if(evt.user.code == 1032) time += 0.1;
                default:
                    break;
                }
            }
            if(win)
            {
                break;
            }

            if(win_cond[now_player]-player_win[now_player] != 0)
            {
                present_image(card_images[98+win_cond[now_player]-player_win[now_player]-1], 0.379, 0.58, 0.25, 0.11);
            }
            print_back_set_ver(cardlistcount[ (now_player+3)%4 ],0.08);
            print_back_set_hor(cardlistcount[ (now_player+2)%4 ],0.1);
            print_back_set_ver(cardlistcount[ (now_player+1)%4 ],0.81);
            print_middle();
            print_card_set(cardlist[now_player],cardlistcount[now_player],0.7);
            present_image(card_images[94+now_player], 0, 0, 0.2, 0.15);
            if(selected_card!=-1)
            {
                present_image(card_images[selected_card],0.2+selected_rmv-0.025,0.65,0.15,0.3);
            }
            print_trump(king_type);
            SDL_RenderPresent(main_renderer);
        }
        return quit;
    }


    void select_button(int x,int y,int &button_selecto)
    {
        printf("player %d select button id=%d\n",now_player,button_select);
        if(button_select == 56 && button_count == 5)
        {
            selec_stack++;
            now_player = (now_player+1)%4;
        }
        else if(button_select == 56)
        {
            selec_stack++;
            now_player = (now_player+1)%4;
        }
        else if(button_select != -1)
        {
            able_decided = true;
            selec_stack=0;
            button_count = ((57+28)-button_select > 4 )?4:((57+28)-button_select);
            for(int i=1;i<(57+28)-button_select&&i<4;i++)
            {
                selec[i] = button_select+i;
            }
            now_player = (now_player+1)%4;
            button_selecto = button_select;
        }

        if(able_decided)
        {
            if(selec_stack == 3)
            {
                decided = true;
            }
        }

        if(decided)
        {
            king_type = (button_selecto-57)%4;
            org_king = king_type;

            int winner1 = (button_selecto-57)/4+7;

            for(int i=now_player;i<now_player+4;i++)
            {
                win_cond[i%4] = ((i-now_player)%2==0)?winner1:(14-winner1);
                printf("player %d win cond = %d\n",i%4,win_cond[i%4]);
            }
            printf("decide king = %d\n",king_type);
        }
    }

    void select_motion(int x,int y)
    {
        double rmove = 0.8/button_count;
        button_select = -1;
        for(int i=0;i<button_count;i++)
        {
            if(x >= partital(0.1+i*rmove,windows_width) && x <= partital(0.1+(i+1)*rmove,windows_width) && y >= partital(0.45,windows_height) && y <= partital(0.6,windows_height))
            {
                button_select = selec[i];
            }
        }
    }

    void preset_select(int *button,int button_count)
    {
        double wid = 0.8/button_count;
        for(int i=0;i<button_count;i++)
        {
            // y = 0.45 , 0.6
            // x = 0.1 , 0.9
            present_image(card_images[button[i]],0.1+wid*i,0.45,wid,0.15);
        }
    }

    int begin_motion(int x, int y)
    {
        if(x>=39&&x<=345&&y>=541&&y<=680)
        {
            return 1;
        }
        else if(x>=744&&x<=1061&&y>=541&&y<=680)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    int begin_button(int x, int y)
    {
        printf("%d,%d\n",x,y);
        if(x>=39&&x<=345&&y>=541&&y<=680)
        {
            return 1;
        }
        else if(x>=744&&x<=1061&&y>=541&&y<=680)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    bool begin_loop()
    {
        bool qui = false;
        button_count = 2;
        int move = -1;
        int sel = -1;

        while(!qui)
        {
            SDL_Event evt;
            present_image(card_images[111], 0, 0, 1, 1);

            while(SDL_PollEvent(&evt) != 0)
            {
                switch(evt.type)
                {
                case(SDL_QUIT):
                    qui = true;
                    break;
                case(SDL_MOUSEBUTTONDOWN):
                    sel = begin_button(evt.motion.x,evt.motion.y);
                    break;
                case(SDL_MOUSEMOTION):
                    move = begin_motion(evt.motion.x,evt.motion.y);
                    break;
                case(SDL_USEREVENT):
                    if(evt.user.code == 1032) time += 0.1;
                default:
                    break;
                }
            }

            if(move==1)
            {
                present_image(card_images[112], 0, 0.69, 0.379, 0.31);
            }
            else if(move==0)
            {
                present_image(card_images[113], 0.62, 0.69, 0.379, 0.31);
            }
            else if(move== -1)
            {

            }

            if(sel != -1)
            {
                if(sel == 1)
                {
                   ptr = 1;
                }
                else if(sel == 0)
                {
                    ptr = 0;
                }
                break;
            }

            SDL_RenderPresent(main_renderer);

        }

        return qui;
    }

    bool select_loop()
    {
        bool quit = false;
        button_count = 5;
        int button_selecto = -1;
        while(!quit)
        {
            SDL_Event evt;
            present_image(card_images[54], 0, 0, 1, 1);
            while(SDL_PollEvent(&evt) != 0)
            {
                switch(evt.type)
                {
                case(SDL_QUIT):
                    quit = true;
                    break;
                case(SDL_MOUSEBUTTONDOWN):
                    select_button(evt.motion.x,evt.motion.y,button_selecto);
                    break;
                case(SDL_MOUSEMOTION):
                    select_motion(evt.motion.x,evt.motion.y);
                    break;
                case(SDL_USEREVENT):
                    if(evt.user.code == 1032) time += 0.1;
                default:
                    break;
                }
            }

            if(decided)
            {
                break;
            }
            if(button_selecto != -1)
            {
                print_trump((button_selecto-57)%4);
            }
            else
            {
                print_trump(button_selecto);
            }
            present_image(card_images[94+now_player], 0, 0, 0.2, 0.15);
            preset_select(selec,button_count);
            SDL_RenderPresent(main_renderer);
        }
        return quit;
    }

    int end_button(int x,int y)
    {
        printf("partion = (%d,%d)\n",x,y);
        if(x>=26&&x<=262&&y>=563&&y<=684)
        {
            //present_image(card_images[92], 0.0333, 0.782, 0.23, 0.17);
            return 0;
        }
        if(x>=277&&x<=518&&y>=564&&y<=680)
        {
            //present_image(card_images[93], 0.0333, 0.782, 0.23, 0.17);
            return 1;
        }
        return -1;
    }

    int end_motion(int x,int y)
    {

        if(x>=26&&x<=262&&y>=563&&y<=684)
        {
            return 0;
        }
        else if(x>=277&&x<=518&&y>=564&&y<=680)
        {
            return 1;
        }
        else
        {
            return -1;
        }

    }


    bool end_loop(bool &q)
    {
        q = false;
        int sel = -1;
        int dle = -1;
        while(!q)
        {
            SDL_Event evt;

            if(player_win[0] >= win_cond[0] || player_win[2]>=win_cond[2])
            {
                present_image(card_images[90], 0, 0, 1, 1);
            }
            else
            {
                present_image(card_images[91], 0, 0, 1, 1);
            }

            while(SDL_PollEvent(&evt) != 0)
            {
                switch(evt.type)
                {
                case(SDL_QUIT):
                    q = true;
                    break;
                case(SDL_MOUSEBUTTONDOWN):
                    sel = end_button(evt.motion.x,evt.motion.y);
                    break;
                case(SDL_MOUSEMOTION):
                    dle = end_motion(evt.motion.x,evt.motion.y);
                    break;
                case(SDL_USEREVENT):
                    if(evt.user.code == 1032) time += 0.1;
                default:
                    break;
                }
            }
            if(dle == 0)
            {
                present_image(card_images[92], 0.0333-0.03, 0.782-0.02, 0.23+0.06, 0.17+0.04);
            }
            else if(dle == 1)
            {
                present_image(card_images[93], 0.26-0.03, 0.782-0.02, 0.23+0.06, 0.17+0.04);
            }

            if(sel != -1)
            {
                break;
            }

            SDL_RenderPresent(main_renderer);
        }

        if(sel == 1)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void exec(SDL_Window* mwindow, SDL_Surface* msurface, SDL_Renderer* mrenderer, const char *title, int window_height,int window_width,std::string *icard_paths,int icard_count)
    {
    	
        printf("you execute %s\n",title);
        windows_height = window_height;
        windows_width = window_width;

        main_window = mwindow;
        main_surface = msurface;
        main_renderer = mrenderer;

        //init(title);

        time = 0.0;
        SDL_TimerID my_timer_id = SDL_AddTimer(100 , timer_callback , NULL);

        card_count = icard_count;
        card_paths = icard_paths;
        load_card();
        bool q=false;

        do
        {
            if(q)
            {
                break;
            }

            now_player = rand()%4;
            servecard();
            king_type = -1;
            button_select=-1;
            for(int i=56;i<=60;i++)
            {
                selec[i-56] = i;
            }
            selec_stack=0;
            decided = false;
            able_decided = false;

            if(begin_loop())
            {
                break;
            }

            if(ptr == 0)
            {
                if(select_loop())
                {
                    break;
                }

                now_player = (now_player+1)%4;

                if(main_loop())
                {
                    break;
                }
                if(!end_loop(q)){
                	break;
				}
            }

            else if(ptr == 1)
            {
            	texas weibor;
            	bool end_or_resume = weibor.holdem();
            	
				if(!end_or_resume){
					std::cout << "exit"<<std::endl;
					break;
				}
				else{
					std::cout<<"resume"<<std::endl;
				}
            }

        }while(1);

        close();
    }

};

int main( int argc, char* args[] )
{
    //SDL_Window* main_window;  gWindow
    SDL_Surface* main_surface;
    //SDL_Renderer* main_renderer;  gRenderer

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER );
    IMG_Init( IMG_INIT_PNG );
    gWindow = SDL_CreateWindow( "card game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_SHOWN );
    main_surface = SDL_GetWindowSurface(gWindow);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	game g;
	std::string card_path[] = {"pictures/club2.png","pictures/diamond2.png","pictures/heart2.png","pictures/spade2.png",
                                "pictures/club3.png","pictures/diamond3.png","pictures/heart3.png","pictures/spade3.png",
                                "pictures/club4.png","pictures/diamond4.png","pictures/heart4.png","pictures/spade4.png",
                                "pictures/club5.png","pictures/diamond5.png","pictures/heart5.png","pictures/spade5.png",
                                "pictures/club6.png","pictures/diamond6.png","pictures/heart6.png","pictures/spade6.png",
                                "pictures/club7.png","pictures/diamond7.png","pictures/heart7.png","pictures/spade7.png",
                                "pictures/club8.png","pictures/diamond8.png","pictures/heart8.png","pictures/spade8.png",
                                "pictures/club9.png","pictures/diamond9.png","pictures/heart9.png","pictures/spade9.png",
                                "pictures/club10.png","pictures/diamond10.png","pictures/heart10.png","pictures/spade10.png",
                                "pictures/club11.png","pictures/diamond11.png","pictures/heart11.png","pictures/spade11.png",
                                "pictures/club12.png","pictures/diamond12.png","pictures/heart12.png","pictures/spade12.png",
                                "pictures/club13.png","pictures/diamond13.png","pictures/heart13.png","pictures/spade13.png",
                                "pictures/club1.png","pictures/diamond1.png","pictures/heart1.png","pictures/spade1.png",

                                "pictures/card.png","pictures/texaspokerdesk.png","pictures/otherdesk.png","pictures/card2.png",
                                "pictures/skip.png",

                                "pictures/chooseclub1.png","pictures/choosediamond1.png","pictures/chooseheart1.png","pictures/choosespade1.png",
                                "pictures/chooseclub2.png","pictures/choosediamond2.png","pictures/chooseheart2.png","pictures/choosespade2.png",
                                "pictures/chooseclub3.png","pictures/choosediamond3.png","pictures/chooseheart3.png","pictures/choosespade3.png",
                                "pictures/chooseclub4.png","pictures/choosediamond4.png","pictures/chooseheart4.png","pictures/choosespade4.png",
                                "pictures/chooseclub5.png","pictures/choosediamond5.png","pictures/chooseheart5.png","pictures/choosespade5.png",
                                "pictures/chooseclub6.png","pictures/choosediamond6.png","pictures/chooseheart6.png","pictures/choosespade6.png",
                                "pictures/chooseclub7.png","pictures/choosediamond7.png","pictures/chooseheart7.png","pictures/choosespade7.png",

                                "pictures/trumpclub.png","pictures/trumpdiamond.png","pictures/trumpheart.png","pictures/trumpspade.png",
                                "pictures/trump.png",

                                "pictures/player13win.png","pictures/player24win.png","pictures/resume.png","pictures/exit.png",

                                "pictures/player1.png","pictures/player2.png","pictures/player3.png","pictures/player4.png",

                                "pictures/remain1.png","pictures/remain2.png","pictures/remain3.png","pictures/remain4.png",
                                "pictures/remain5.png","pictures/remain6.png","pictures/remain7.png","pictures/remain8.png",
                                "pictures/remain9.png","pictures/remain10.png","pictures/remain11.png","pictures/remain12.png",
                                "pictures/remain13.png",
                                "pictures/initial.png","pictures/choosetexas.png","pictures/choosebridge.png"
                                };
	g.exec(gWindow, main_surface, gRenderer, "contract bridge",720,1080,card_path,52+5+28+5+2+2+4+13+3);
	
	return 0;

}

