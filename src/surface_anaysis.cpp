#include "surface_anaysis.hpp"

using namespace std;

inline char Surface_Anaysis::classification_from_color(double r, double g, double b)
{
    double best_score {.75*r+.75*b-.0625};
    char best_classification {'-'};
    if (g > best_score)
    {
        best_score = g;
        best_classification = '+';
    }
    if (r > best_score)
    {
        best_score = r;
        best_classification = 'i';
    }
    if (b > best_score)
    {
        best_score = b;
        best_classification = 'o';
    }
    return best_classification;
};

inline void Surface_Anaysis::make_face(Vertex * vertex1, Vertex * vertex2, Vertex * vertex3)
{
    double a, b, c, s, area;
    auto ptr = vertex1->touching.find(vertex2);
    if(ptr == vertex1->touching.end())
    {
        a = vertex1->distance_to(vertex2);
        vertex1->touching.emplace(vertex2, a);
        vertex2->touching.emplace(vertex1, a);
    }
    else
        a = ptr->second;
    ptr = vertex2->touching.find(vertex3);
    if(ptr == vertex2->touching.end())
    {
        b = vertex2->distance_to(vertex3);
        vertex2->touching.emplace(vertex3, b);
        vertex3->touching.emplace(vertex2, b);
    }
    else
        b = ptr->second;
    ptr = vertex3->touching.find(vertex1);
    if(ptr == vertex3->touching.end())
    {
        c = vertex3->distance_to(vertex1);
        vertex3->touching.emplace(vertex1, c);
        vertex1->touching.emplace(vertex3, c);
    }
    else
        c = ptr->second;
    s = (a+b+c)/2;
    area = sqrt(s*(s-a)*(s-b)*(s-c))/3;
    vertex1->area += area;
    vertex2->area += area;
    vertex3->area += area;
};

inline void Surface_Anaysis::read_wrl()
{
    ifstream file("proteins/"+protein_name+"/"+protein_name+".wrl");
    if (!file)
        throw runtime_error("Error opening file.");
    list<Vertex *> vertices_data;
    list<char> color_data;
    double best_x, best_y, best_z {0};
    string line, a, b, c;
    while (line!="point")
        file>>line;
    file>>line;
    unordered_set<Vertex *, vertex_hash, vertex_hash> vertices_dict;
    while (true)
    {
        file>>a;
        if (a=="]")
            break;
        file>>b>>c;
        c=c.substr(0, c.length()-1);
        Vertex vertex{stod(a), stod(b), stod(c)};
        auto search = vertices_dict.find(&vertex);
        if (search != vertices_dict.end())
        {
            vertices_data.push_back(*search);
            continue;
        }
        vertices.push_back(vertex);
        vertices_dict.emplace(&vertices.back());
        vertices_data.push_back(&vertices.back());
        if (select_type == custom && best_y<vertex.y)
        {
            best_x=vertex.x;
            best_y=vertex.y;
            best_z=vertex.z;
        }
    }
    while (line != "color")
        file>>line;
    file>>line>>line>>line>>line;
    while (true)
    {
        file>>a;
        if (a=="]")
            break;
        file>>b>>c;
        c=c.substr(0, 6);
        color_data.push_back(classification_from_color(stod(a), stod(b), stod(c)));
    }
    if (select_type == custom)
    {
        ifstream file("proteins/"+protein_name+"/.protein");
        file>>line;
        selector_x += best_x - stod(line);
        file>>line;
        selector_y += best_y - stod(line);
        file>>line;
        selector_z += best_z - stod(line);
    }
    list<Vertex *>::iterator vertex_ptr{vertices_data.begin()};
    list<char>::iterator color_ptr{color_data.begin()};
    Vertex * vertex1, * vertex2, * vertex3;
    while (vertex_ptr != vertices_data.end())
    {
        (vertex1 = *vertex_ptr)->classification = *color_ptr;
        ++vertex_ptr;
        ++color_ptr;
        (vertex2 = *vertex_ptr)->classification = *color_ptr;
        ++vertex_ptr;
        ++color_ptr;
        (vertex3 = *vertex_ptr)->classification = *color_ptr;
        ++vertex_ptr;
        ++color_ptr;
        make_face(vertex1, vertex2, vertex3);
    }
};

inline void Surface_Anaysis::build_patches()
{
    queue<Vertex *> edge;
    for (Vertex & start : vertices)
    {
        if (start.patch != nullptr)
            continue;
        start.patch = &patches.emplace_back(start.classification);
        edge.push(&start);
        for (; !edge.empty(); edge.pop())
        {
            bool rim {false};
            for (std::pair<Vertex *const, double> & touch : edge.front()->touching)
                switch ( (touch.first->classification==start.patch->classification)*2 + (touch.first->patch == nullptr || touch.first->check == 3) )
                {
                case 0:
                    if (start.patch->touching.empty() || !(find(start.patch->touching.begin(), start.patch->touching.end(), touch.first->patch) != start.patch->touching.end()))
                    {
                        start.patch->touching.push_back(touch.first->patch);
                        touch.first->patch->touching.push_back(start.patch);
                    }
                case 1:
                    rim=true;
                    break;
                case 3: 
                    touch.first->patch=start.patch;
                    edge.push(touch.first);
                }
            (rim ? start.patch->rim : start.patch->core).insert(edge.front());
            start.patch->area += edge.front()->area;
        }
        if (start.patch->core.empty())
        {
            for (Patch * & patch : start.patch->touching)
                patch->touching.pop_back();
            for (Vertex * const & vertex : start.patch->rim)
                vertex->check = 3;
            patches.pop_back();
        }
    }
}

inline void Surface_Anaysis::build_patches_distances()
{
    set<Vertex *, vertex_set> edge;
    for (Patch & patch : patches)
    {
        unsigned int check {0};
        for (Vertex * const & rim : patch.rim)
        {
            ++rim->check;
            for (pair<Vertex * const, double> & touch : rim->touching)
                if (touch.first->classification==patch.classification)
                {
                    touch.first->distance=touch.second;
                    edge.emplace(touch.first);
                }
            for (auto next = edge.begin(); !edge.empty(); edge.erase(next))
            {
                next = edge.begin();
                patch.distances.emplace(make_pair(*next, rim), (*next)->distance);
                ++(*next)->check;
                for (pair<Vertex *const, double> & touch : (*next)->touching)
                    if (touch.first->check == check && touch.first->classification==patch.classification)
                    {
                        auto search {find(edge.begin(), edge.end(), touch.first)};
                        if (search == edge.end())
                        {
                            touch.first->distance = touch.second + (*next)->distance;
                            edge.emplace(touch.first);
                        }
                        else if (touch.first->distance > (*next)->distance + touch.second)
                        {
                            auto extract {edge.extract(touch.first)};
                            extract.value()->distance = (*next)->distance + touch.second;
                            edge.insert(move(extract));
                        }
                    }
            }
            ++check;
        }
    }
}

inline void Surface_Anaysis::build_centers()
{
    for (Patch & patch : patches)
    {
        if (patch.rim.empty())
            continue;
        double best_score{INFINITY};
        Vertex * best_vertex{nullptr};
        for (Vertex * const & vertex : patch.core)
        {
            double mean {0}, absolute_deviation {0}, mean_absolute_deviation {0};
            for (Vertex * const & rim : patch.rim)
                mean += patch.distances[make_pair(rim, vertex)];
            mean /= patch.rim.size();
            for (Vertex * const & rim : patch.rim)
                absolute_deviation += pow(patch.distances[make_pair(rim, vertex)] - mean, 2);
            mean_absolute_deviation = sqrt(absolute_deviation)/patch.rim.size();
            if (mean_absolute_deviation < best_score)
            {
                best_score = mean_absolute_deviation;
                best_vertex = vertex;
            }
        }
        for (Vertex * const & vertex : patch.rim)
        {
            double mean {0}, absolute_deviation {0}, mean_absolute_deviation {0};
            for (Vertex * const & rim : patch.rim)
                if (rim != vertex)
                    mean += patch.distances[make_pair(rim, vertex)];
            mean /= patch.rim.size()-1;
            for (Vertex * const & rim : patch.rim)
                if (rim != vertex)
                    absolute_deviation += pow(patch.distances[make_pair(rim, vertex)] - mean, 2);
            mean_absolute_deviation = sqrt(absolute_deviation)/(patch.rim.size()-1);
            if (mean_absolute_deviation < best_score)
            {
                best_score = mean_absolute_deviation;
                best_vertex = vertex;
            }
        }
        patch.center_vertex = best_vertex;
    }
}

inline void Surface_Anaysis::filter_patches()
{
    Patch * best_patch {nullptr};
    double best_score {select_type==outer ? -INFINITY : INFINITY};
    if (select_type==outer)
        patches.remove_if(
            [=, &best_patch, &best_score](Patch & value) -> bool 
            {
                if (!value.center_vertex)
                    return true;
                if(value.center_vertex->y>best_score)
                {
                    best_score = value.center_vertex->y;
                    best_patch = &value;
                }
                return false;
            }
        );
    else
        patches.remove_if(
            [=, &best_patch, &best_score](Patch & value) -> bool 
            {
                if (!value.center_vertex)
                    return true;
                double dist {value.center_vertex->distance_to(selector_x, selector_y, selector_z)};
                if (dist<selector_radius)
                {
                    if(dist<best_score)
                    {
                        best_score = dist;
                        best_patch = &value;
                    }
                    return false;
                }
                return true;
            }
        );
    best_patch->selected=true;
}

inline void Surface_Anaysis::build_surfaces()
{
    queue<Patch *> edge;
    if (select_type==all)
        for (Patch & patch : patches)
        {
            if (patch.surface)
                continue;
            patch.surface = &(surfaces.emplace_back(list<Patch *> {&patch}));
            for (edge.push(&patch); !edge.empty(); edge.pop())
                for (Patch * & touch : edge.front()->touching)
                    if (touch->surface)
                    {
                        (touch->surface=patch.surface)->push_back(touch);
                        touch->center_vertex->is_center=true;
                        edge.push(touch);
                    }
        }
    else
        for (Patch & patch : patches)
        {
            if (!patch.selected)
                continue;
            patch.surface = &(surfaces.emplace_back(list<Patch *> {&patch}));
            for (edge.push(&patch); !edge.empty(); edge.pop())
                for (Patch * & touch : edge.front()->touching)
                    if (!touch->surface)
                    {
                        (touch->surface=patch.surface)->push_back(touch);
                        touch->center_vertex->is_center=true;
                        edge.push(touch);
                    }
            break;
        }
}

inline void Surface_Anaysis::build_distances()
{
    for (list<Patch *> & surface : surfaces)
    {
        set<Vertex *, vertex_set> edge;
        unsigned int to_check {surface.size()+1}, checked {1};
        Vertex * start {(*surface.begin())->center_vertex};
        for (start->check=0; --to_check; checked=1)
        {
            start->is_center=false;
            for (pair<Vertex * const, double> & touch : start->touching)
            {
                touch.first->distance = touch.second;
                edge.emplace(touch.first);
            }
            for (auto next = edge.begin(); !edge.empty(); edge.erase(next))
            {
                (*(next = edge.begin()))->check = surface.size()-to_check;
                if ((*next)->is_center)
                {
                    distances.emplace(make_pair(start, *next), (*next)->distance);
                    if (++checked==to_check)
                    {
                        start = *next;
                        break;
                    }
                }
                for (pair<Vertex *const, double> & touch : (*next)->touching)
                {
                    if (surface.size()-to_check != touch.first->check)
                    {
                        auto search {find(edge.begin(), edge.end(), touch.first)};
                        if (search == edge.end())
                        {
                            touch.first->distance = touch.second + (*next)->distance;
                            edge.emplace(touch.first);
                        }
                        else if (touch.first->distance > touch.second + (*next)->distance)
                        {
                            auto extract = edge.extract(search);
                            extract.value()->distance = touch.second + (*next)->distance;
                            edge.insert(move(extract));
                        }
                    }
                }
            }
        }
    }
}

inline void const Surface_Anaysis::write_files()
{
    if (!wxDirExists("data/"+data_name))
        wxMkdir("data/"+data_name);
    ofstream sizes_file {"data/"+data_name+"/"+"sizes"};
    ofstream distances_file {"data/"+data_name+"/"+"distances"};
    ofstream data_file {"data/"+data_name+"/"+".data"};
    data_file<<protein_name<<"\n";
    switch (select_type)
    {
    case all:
        data_file<<"all"<<"\n";
        break;
    case outer:
        data_file<<"outer"<<"\n";
        break;
    case custom:
        data_file<<"custom"<<"\n";
        break;
    }
    for (auto & surface : surfaces)
        for (auto first = surface.begin(); first != surface.end(); ++first)
        {
            auto second = first;
            for (++second; second != surface.end(); ++second)
                if ((*first)->classification == (*second)->classification)
                    distances_file<<(*first)->classification<<" "<<distances[make_pair((*first)->center_vertex, (*second)->center_vertex)]<<" "<<"\n";
            sizes_file<<(*first)->classification<<" "<<(*first)->area<<"\n";
        }
}


void Surface_Anaysis::run()
{
    auto start = high_resolution_clock::now();
    read_wrl();
    cout << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << endl;
    build_patches();
    cout << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << endl;
    build_patches_distances();
    cout << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << endl;
    build_centers();
    cout << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << endl;
    if (select_type != all)
    {
        filter_patches();
        cout << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << endl;
    }
    build_surfaces();
    cout << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << endl;
    build_distances();
    cout << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << endl;
    write_files();
    cout << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << endl;
};

