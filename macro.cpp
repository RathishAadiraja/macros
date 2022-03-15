#include<iostream>
#include<vector>
#include<map>
#include<iterator>
#include<tuple>
#include<fstream>


using namespace std;

class ReadFile{

private:
    string filepath;
    string filetxt;
    bool is_valid;

public:

    bool get_path();
    bool load_file();
    string get_whole_text();
    void check_text();

};

bool ReadFile::get_path(){

    cout << "Please write the path of the text file." << endl;
    cin >> filepath;

    if (FILE *file = fopen(filepath.c_str(), "r")) {
        fclose(file);
        is_valid = true;
        return true;
    } else {
        is_valid = false;
        return false;
    }

}



bool ReadFile::load_file(){

    if(is_valid){

        string stringfile, tmp;

        ifstream input(filepath);

        while(!input.eof()) {
            getline(input, tmp);
            stringfile += tmp;
            stringfile += "\n";
        }

        filetxt = stringfile;
        return true;


    }else{

        cout << "Incorrect file name or file path" << endl;
        cout << "-------------------------------------------------------------------------------------" << endl << endl;
        return false;
    }


}



string ReadFile::get_whole_text(){

    if(is_valid){
        return filetxt;
    }else{

        return "";
    }


}

void ReadFile::check_text(){

    if(is_valid){

        cout << filetxt;

    }else{

        cout << "File not read successfully" << endl;;

    }

}


class CheckMacro{

private:

    string check_text;
    string final_text;
    vector<string> macrodefs;
    multimap<tuple<string,int>,string> map_def;



public:

    void set_text(string p_check_text);
    string get_text(){return check_text;}
    bool check_name(string p_name);
    int check_parameters(string p_parameters);
    multimap<tuple<string,int>,string> seperate_each_macrodefs();
    void print_final_vector();
    void print_multimap();


};


void CheckMacro::set_text(string p_check_text){

    if(p_check_text != ""){

        check_text = p_check_text;
    }

}


bool CheckMacro::check_name(string p_name){

    for(string::size_type i = 0; i < p_name.size(); i++) {

        int ascii_val = int(p_name[i]);

        if( (ascii_val >= 48 && ascii_val <= 57) ||
            (ascii_val >= 65 && ascii_val <= 90) ||
            (ascii_val >= 97 && ascii_val <= 122) ||
            (ascii_val == 95)){

            // do nothing

        }
        else{

            return false;
        }


    }

    return true;


}


int CheckMacro::check_parameters(string p_parameters){

    int symbol_count = 0;
    int comma_count = 0;

    for(string::size_type i = 0; i < p_parameters.size(); i++) {



        if((int(p_parameters[i]) >= 48 && int(p_parameters[i]) <= 57) ||
           (int(p_parameters[i]) == 38)||
           (int(p_parameters[i]) == 44)||
           (int(p_parameters[i]) == 32)){

            if(int(p_parameters[i]) == 38){

                i++;

                while(int(p_parameters[i]) >= 48 && int(p_parameters[i]) <= 57){

                    if(i == p_parameters.size()-1){
                        symbol_count++;
                        break;
                    }else{

                        i++;

                    }
                }

                if((int(p_parameters[i])== 32)|| (int(p_parameters[i]) == 44)){

                    symbol_count++;
                }
            }else if(int(p_parameters[i]) >= 47 && int(p_parameters[i]) <= 57){

                return 0;
            }

            if((int(p_parameters[i]) == 44)){

                comma_count++;
            }

        }else{


            return 0;

        }

    }



    if(symbol_count != comma_count+1){

        return 0;
    }

    return symbol_count;


}

multimap<tuple<string,int>,string> CheckMacro::seperate_each_macrodefs(){



    for(string::size_type i = 0; i < check_text.size(); i++) {

        if(check_text[i] == '#'){

            int str_num = i;
            string local_str = "";
            local_str += check_text[i];

            i++;
            string name = "";
            while(check_text[i] != '('){

                name += check_text[i];
                i++;
            }

            if(check_name(name)){

                local_str += name;

                local_str += check_text[i];
                i++;

                string check_param = "";
                while(check_text[i] != ')'){

                    check_param += check_text[i];
                    i++;
                }

                int param_count = check_parameters(check_param);

                if(param_count > 0){

                    local_str += check_param;

                    local_str += check_text[i];
                    i++;

                    bool space_format = true;
                    while(check_text[i] != '{'){

                        if(int(check_text[i]) == 32)
                        {
                            local_str += check_text[i];
                            i++;
                        }else{

                            space_format = false;
                            break;
                        }
                    }

                    if(space_format){

                        if(check_text[i] == '{'){
                            local_str += check_text[i];
                            i++;
                            while(check_text[i] != '}'){

                                local_str += check_text[i];
                                i++;
                            }
                            local_str += '}';
                            macrodefs.push_back(local_str);

                            tuple<string,int> temp;
                            temp = make_tuple(name,param_count);
                            map_def.insert({temp,local_str});
                        }

                    }

                }

            }else{

                i = str_num;

            }
        }
    }

    return map_def;

}


void CheckMacro::print_final_vector(){

    for (vector<string>::const_iterator i = macrodefs.begin(); i != macrodefs.end(); ++i){

        cout << *i << endl;
    }

}


void CheckMacro::print_multimap(){


    for(auto it = map_def.begin(); it!=map_def.end(); ++it){

        cout << get<0>(it->first) << "----" << get<1>(it->first);

        cout<< "\t";

        cout << it->second << endl;

        cout << endl ;
        cout << "--------------------------------------------" << endl;
    }


}



class MapMacros{

private:

    multimap<tuple<string,int>,string> to_map;
    tuple<string,int> macro_name;
    multimap<string,int> macro_calls;
    multimap<tuple<string,int>,multimap<string,int>> mapped_macros;

public:

    void set_to_map(multimap<tuple<string,int>,string> p_to_map){to_map = p_to_map;}
    multimap<tuple<string,int>,string> get_to_map(){return to_map;}
    void map_macros();
    int check_param_count(string p_str);
    void show_map();
    bool check_valid_calls();
    bool check_inf_recursion();
    void show_results();

};



int MapMacros::check_param_count(string p_str){

    int parameter_count = 1;

    for(string::size_type i = 0; i < p_str.size(); i++) {

        if((int(p_str[i]) == 44)){

            parameter_count++;
        }

    }

    return parameter_count;



}

void MapMacros::map_macros(){

    for(auto it = to_map.begin(); it!= to_map.end(); ++it){

        macro_name = it->first;

        string temp_def = it->second;

        for(string::size_type i = 0; i < temp_def.size(); i++) {

            bool while_1 = true;
            while(temp_def[i] != '$'){

                if(i+2 == temp_def.size()){
                    while_1 = false;
                    i = temp_def.size();
                    break;
                }else{
                    i++;
                }
            }
            if(while_1){

                i++;

                string call_name = "";
                bool while_2 = true;
                while(temp_def[i] != '('){

                    if(i+2 == temp_def.size()){
                        while_2 = false;
                        break;
                    }else{
                        call_name += temp_def[i];
                        i++;
                    }
                }

                if(while_2){

                    if(temp_def[i] == '('){

                    i++;
                    }
                    string check_str = "";
                    bool while_3 = true;
                    while(temp_def[i] != ')'){

                        if(i+1 == temp_def.size()){
                            while_3 = false;
                            break;
                        }else{
                            check_str += temp_def[i];
                            i++;
                        }
                    }

                    if(while_3){
                        int par_count = check_param_count(check_str);

                        macro_calls.insert({call_name,par_count});

                    }
                }
            }

        }

        mapped_macros.insert({macro_name,macro_calls});
        macro_calls.clear();


    }




}


void MapMacros::show_map(){


    if(!mapped_macros.empty()){
        for(auto it = mapped_macros.begin(); it!= mapped_macros.end(); ++it){

            cout << get<0>(it->first) << "----" << get<1>(it->first);

            cout<< "\t\t";

            for(auto it2 = it->second.begin(); it2!= it->second.end(); ++it2){
                cout << it2->first << "----" << it2->second;
                cout << endl <<"\t\t\t" ;

            }
            cout << endl;
            cout << "--------------------------------------------" << endl;
        }

    }else{

        cout << "No macros detected" << endl;

    }

}


bool MapMacros::check_valid_calls(){

    if(!mapped_macros.empty()){

        for(auto it = mapped_macros.begin(); it!= mapped_macros.end(); ++it){

            string it_temp_name = get<0>(it->first);

            for(auto it2 = it->second.begin(); it2!= it->second.end(); ++it2){


                bool check_if_there = false;

                for(auto it3 = to_map.begin(); it3!= to_map.end(); ++it3){

                    if(get<0>(it3->first) == it2->first && get<1>(it3->first) == it2->second){

                        check_if_there = true;
                    }

                }

                if(!check_if_there){

                    for(auto it4 = to_map.begin(); it4!= to_map.end(); ++it4){

                        if(get<0>(it->first) == get<0>(it4->first) && get<1>(it->first) == get<1>(it4->first)){

                            cout << "Invalid macro call detected in macro " << "'"<<get<0>(it->first)<<"'" <<
                             " with " << get<1>(it->first) << " parameter" << endl;
                            cout << it4->second << endl;

                            return false;
                        }

                    }

                }

            }

        }

        return true;
    }else{

        return false;
    }


}

bool MapMacros::check_inf_recursion(){

    if(check_valid_calls()){

        multimap<tuple<string,int>,multimap<string,int>> duplicate_mapped_macros = mapped_macros;

        for(auto it = mapped_macros.begin(); it!= mapped_macros.end(); ++it){

            for(auto it2 = it->second.begin(); it2!= it->second.end(); ++it2){

                for(auto it3 = duplicate_mapped_macros.begin(); it3!= duplicate_mapped_macros.end(); ++it3){

                    if(get<0>(it3->first) == it2->first && get<1>(it3->first) == it2->second){

                        for(auto it4 = it3->second.begin(); it4!= it3->second.end(); ++it4){

                            if(it4->first == get<0>(it->first) && it4->second == get<1>(it->first))
                            {

                                for(auto it5 = to_map.begin(); it5!= to_map.end(); ++it5){

                                    if(get<0>(it->first) == get<0>(it5->first) && get<1>(it->first) == get<1>(it5->first)){

                                        cout << "Infinite recursion detected in macro " << "'" <<get<0>(it->first)<< "'" <<
                                         " with " << get<1>(it->first) << " parameter" << endl;
                                        cout << it5->second << endl;

                                    }

                                    if(get<0>(it3->first) == get<0>(it5->first) && get<1>(it3->first) == get<1>(it5->first)){

                                        cout << endl << "Infinite recursion detected in macro " << "'" <<get<0>(it3->first)<< "'" <<
                                         " with " << get<1>(it3->first) << " parameter" << endl;
                                        cout << it5->second << endl;

                                    }
                                }

                                return true;
                            }
                        }

                    }
                }

            }


        }
        cout << "No possible Infinite Recursion detected" << endl;
        return false;
    }else{

        return false;
    }

}


int main(){


    ReadFile myfile;
    bool load_check = false;

    while(!load_check){
        myfile.get_path();
        load_check = myfile.load_file();
    }

    if(myfile.load_file()){

        CheckMacro mymacro;
        multimap<tuple<string,int>,string> all_macros;
        mymacro.set_text(myfile.get_whole_text());
        all_macros = mymacro.seperate_each_macrodefs();
        mymacro.print_final_vector();
        cout << endl << endl;


        if(!all_macros.empty()){

            MapMacros my_map;
            my_map.set_to_map(all_macros);
            my_map.map_macros();
            my_map.show_map();
            my_map.check_inf_recursion();

        }else{

            cout << "No Macros Detected" << endl;

        }

    }

    return 0;


}
