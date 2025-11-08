#include "hashmap_of_hashmaps.h"

hashmap_of_hashmaps::hashmap_of_hashmaps()
{
    filter["test"];
}

void hashmap_of_hashmaps::add(std::string filt, int value, std::string college_name)
{
    filter[filt][value].push_back(college_name);
}

std::map<int, std::vector<std::string>> *hashmap_of_hashmaps::get_category(std::string filter_name)
{
    auto it = this->filter.find(filter_name);
    if (it == this->filter.end())
    {
        return nullptr;
    }
    return &(it->second);
}

void hashmap_of_hashmaps::print_whole_category(std::string filter_name)
{
    auto filt = get_category(filter_name);
    if (!filt)
    {
        std::cout << "No filter found" << std::endl;
        return;
    }

    std::cout << "Filter: " << filter_name << std::endl;

    for (auto &[value, schools] : *filt)
    {
        std::cout << " " << value << ": ";
        for (int i = 0; i < (int)schools.size(); i++)
        {
            std::cout << schools[i];
            if (i + 1 < (int)schools.size())
            {
                std::cout << "< ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<std::string> hashmap_of_hashmaps::top_n_colleges(std::string filter_name, int n)
{
    std::vector<std::string> output;
    auto filt = get_category(filter_name);
    if (!filt)
    {
        return output;
    }

    for (auto it = filt->rbegin(); it != filt->rend(); ++it)
    {
        const std::vector<std::string> &schools = it->second;
        for (int i = 0; i < (int)schools.size(); i++)
        {
            output.push_back(schools[i]);
            if ((int)output.size() == n)
            {
                return output;
            }
        }
    }
    return output;
}

std::vector<std::string> hashmap_of_hashmaps::bottom_n_colleges(std::string filter_name, int n)
{
    std::vector<std::string> output;
    auto filt = get_category(filter_name);
    if (!filt)
    {
        return output;
    }

    for (auto it = filt->begin(); it != filt->end(); ++it)
    {
        const std::vector<std::string> &schools = it->second;
        for (int i = 0; i < (int)schools.size(); i++)
        {
            output.push_back(schools[i]);
            if ((int)output.size() == n)
            {
                return output;
            }
        }
    }
    return output;
}

bool hashmap_of_hashmaps::load_file(std::string &path, bool has_header)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        std::cout << "Failed to open file " << path << std::endl;
        return false;
    }
    std::string line;
    std::vector<std::string> header_colums;
    if (has_header)
    {
        if (!std::getline(file, line))
        {
            std::cout << "Failed to read header from file " << path << std::endl;
            return false;
        }
        file_split_line(line, header_colums);
    }
    else{
        header_colums.clear();
    }

    while (std::getline(file, line))
    {
        std::vector<std::string> cols;
        file_split_line(line, cols);
        if ((int)cols.size() < 8)
        { // expected 8 columns
            continue;
        }
        std::string college_name = cols[0];
        name_to_state[college_name]   = cols[2];
        name_to_control[college_name] = cols[3];

        for (int i = 1 ; i <= 7; i++)
        {
            std::string categ;
            if (has_header && i < (int)header_colums.size()){
                categ = header_to_category(header_colums[i]);
            } else {
                categ = "col_" + std::to_string(i);
            }

            int int_value = parse_int_safe(cols[i], -1);
            if (int_value != -1) {
                add(categ, int_value, college_name);
                continue;
            }

            float float_value;
            bool check_float = parse_float_safe(cols[i], float_value);
            if (check_float) {
                int scaled = (int)(float_value * 10000.0f);
                add(categ, scaled, college_name);
            }
        }
    }
    return true;
}

std::vector<std::string> hashmap_of_hashmaps::recommend_colleges(
    int n,
    float wTuition, float wAcceptance, float wSAT, float wUndergrad,
    int min_sat,
    const std::string& state_filter,
    const std::string& control_filter,
    int max_tuition,
    float min_acceptance
){
    std::unordered_set<std::string> candidates;
    bool first_constraint = true;

    auto intersect_with = [&](const std::unordered_set<std::string>& incoming){
        if (first_constraint) {
            candidates = incoming;
            first_constraint = false;
            return;
        }
        std::unordered_set<std::string> out;
        for (std::unordered_set<std::string>::const_iterator it = candidates.begin(); it != candidates.end(); ++it) {
            if (incoming.find(*it) != incoming.end()) out.insert(*it);
        }
        candidates.swap(out);
    };

    // SAT >= min_sat
    if (min_sat > 0) {
        const std::map<int, std::vector<std::string> >* m = get_category("SAT scores");
        if (m && !m->empty()) {
            std::unordered_set<std::string> s;
            std::map<int,std::vector<std::string> >::const_iterator it = m->lower_bound(min_sat);
            for (; it != m->end(); ++it) {
                const std::vector<std::string>& v = it->second;
                for (int i = 0; i < (int)v.size(); i++) s.insert(v[i]);
            }
            intersect_with(s);
        }
    }

    // tuition <= max_tuition
    if (max_tuition > 0) {
        const std::map<int, std::vector<std::string> >* m = get_category("tuition");
        if (m && !m->empty()) {
            std::unordered_set<std::string> s;
            std::map<int,std::vector<std::string> >::const_iterator it = m->begin();
            for (; it != m->end() && it->first <= max_tuition; ++it) {
                const std::vector<std::string>& v = it->second;
                for (int i = 0; i < (int)v.size(); i++) s.insert(v[i]);
            }
            intersect_with(s);
        }
    }

    // acceptance >= min_acceptance
    if (min_acceptance > 0.0f) {
        int threshold = (int)(min_acceptance * 10000.0f);
        const std::map<int, std::vector<std::string> >* m = get_category("admission rate");
        if (m && !m->empty()) {
            std::unordered_set<std::string> s;
            std::map<int,std::vector<std::string> >::const_iterator it = m->lower_bound(threshold);
            for (; it != m->end(); ++it) {
                const std::vector<std::string>& v = it->second;
                for (int i = 0; i < (int)v.size(); i++) s.insert(v[i]);
            }
            intersect_with(s);
        }
    }

    // state filter
    if ((int)state_filter.size() > 0) {
        std::string needle = to_lower_copy(state_filter);
        std::unordered_set<std::string> s;
        if (first_constraint) {
            const std::map<int, std::vector<std::string> >* m = get_category("tuition");
            if (m) {
                for (std::map<int,std::vector<std::string> >::const_iterator it = m->begin(); it != m->end(); ++it) {
                    const std::vector<std::string>& v = it->second;
                    for (int i = 0; i < (int)v.size(); i++) s.insert(v[i]);
                }
            }
            first_constraint = false;
        } else {
            s = candidates;
        }

        std::unordered_set<std::string> kept;
        for (std::unordered_set<std::string>::const_iterator it = s.begin(); it != s.end(); ++it) {
            std::unordered_map<std::string,std::string>::const_iterator jt = name_to_state.find(*it);
            if (jt != name_to_state.end()) {
                std::string hay = to_lower_copy(jt->second);
                if ((int)needle.size() == 0 || hay.find(needle) != std::string::npos) {
                    kept.insert(*it);
                }
            }
        }
        candidates.swap(kept);
    }

    // control filter
    if ((int)control_filter.size() > 0) {
        std::string needle = to_lower_copy(control_filter);
        std::unordered_set<std::string> kept;
        for (std::unordered_set<std::string>::const_iterator it = candidates.begin(); it != candidates.end(); ++it) {
            std::unordered_map<std::string,std::string>::const_iterator jt = name_to_control.find(*it);
            if (jt != name_to_control.end()) {
                std::string hay = to_lower_copy(jt->second);
                if ((int)needle.size() == 0 || hay.find(needle) != std::string::npos) {
                    kept.insert(*it);
                }
            }
        }
        candidates.swap(kept);
    }

    if (first_constraint) {
        const std::map<int, std::vector<std::string> >* m = get_category("tuition");
        if (m) {
            for (std::map<int,std::vector<std::string> >::const_iterator it = m->begin(); it != m->end(); ++it) {
                const std::vector<std::string>& v = it->second;
                for (int i = 0; i < (int)v.size(); i++) candidates.insert(v[i]);
            }
        }
    }

    std::vector<std::pair<float,std::string> > scored;
    build_weighted_scores_filtered(scored, wTuition, wAcceptance, wSAT, wUndergrad, candidates);

    std::sort(scored.begin(), scored.end(),
              [](const std::pair<float,std::string>& a, const std::pair<float,std::string>& b){
                  if (a.first == b.first) return a.second < b.second;
                  return a.first > b.first;
              });

    std::vector<std::string> out;
    for (int i = 0; i < (int)scored.size() && (int)out.size() < n; i++) {
        out.push_back(scored[i].second);
    }
    return out;
}

std::string hashmap_of_hashmaps::to_lower_copy(const std::string& s)
{
    std::string t = s;
    for (int i = 0; i < (int)t.size(); i++) {
        char c = t[i];
        if (c >= 'A' && c <= 'Z') t[i] = char(c - 'A' + 'a');
    }
    return t;
}

void hashmap_of_hashmaps::file_split_line(std::string &s, std::vector<std::string> &out)
{
    out.clear();
    std::string cur;
    for (int i = 0; i < (int)s.size(); i++)
    {
        char ch = s[i];
        if (ch == ',')
        {
            out.push_back(cur);
            cur.clear();
        }
        else
        {
            cur.push_back(ch);
        }
    }
    out.push_back(cur);
}

int hashmap_of_hashmaps::parse_int_safe(const std::string &s, int fallback)
{
    int i = 0;
    while (i < (int)s.size() && (s[i] == ' ' || s[i] == '\t'))
        i++;
    int j = (int)s.size() - 1;
    while (j >= 0 && (s[j] == ' ' || s[j] == '\t'))
        j--;
    if (i > j)
    {
        return fallback;
    }
    std::string t = s.substr(i, j - i + 1);
    try
    {
        return std::stoi(t);
    }
    catch (...)
    {
        return fallback;
    }
}

bool hashmap_of_hashmaps::parse_float_safe(const std::string &s, float& out)
{
    int i = 0;
    while (i < (int)s.size() && (s[i] == ' ' || s[i] == '\t'))
        i++;
    int j = (int)s.size() - 1;
    while (j >= 0 && (s[j] == ' ' || s[j] == '\t'))
        j--;
    if (i > j) return false;

    std::string t = s.substr(i, j - i + 1);
    try
    {
        out = std::stof(t);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::string hashmap_of_hashmaps::header_to_category(const std::string& header)
{
    if (header == "sat_score") return "SAT scores";
    if (header == "undergrad_enrollment") return "school size";
    if (header == "tuition") return "tuition";
    if (header == "admission_rate") return "admission rate";
    return header;
}

void hashmap_of_hashmaps::collect_category_values(std::unordered_map<std::string,int>& sat,
                                                  std::unordered_map<std::string,int>& adm,
                                                  std::unordered_map<std::string,int>& tui,
                                                  std::unordered_map<std::string,int>& siz,
                                                  int& sat_min, int& sat_max,
                                                  int& adm_min, int& adm_max,
                                                  int& tui_min, int& tui_max,
                                                  int& siz_min, int& siz_max)
{
    sat.clear(); adm.clear(); tui.clear(); siz.clear();

    sat_min =  900000000; sat_max = -900000000;
    adm_min =  900000000; adm_max = -900000000;
    tui_min =  900000000; tui_max = -900000000;
    siz_min =  900000000; siz_max = -900000000;

    const std::map<int, std::vector<std::string> >* m_sat = get_category("SAT scores");
    const std::map<int, std::vector<std::string> >* m_adm = get_category("admission rate");
    const std::map<int, std::vector<std::string> >* m_tui = get_category("tuition");
    const std::map<int, std::vector<std::string> >* m_siz = get_category("school size");

    if (m_sat && !m_sat->empty()) {
        for (std::map<int,std::vector<std::string> >::const_reverse_iterator it = m_sat->rbegin();
             it != m_sat->rend(); ++it) {
            int val = it->first;
            if (val < sat_min) sat_min = val;
            if (val > sat_max) sat_max = val;
            const std::vector<std::string>& vec = it->second;
            for (int i = 0; i < (int)vec.size(); i++) {
                if (sat.find(vec[i]) == sat.end()) sat[vec[i]] = val;
            }
        }
    }

    if (m_adm && !m_adm->empty()) {
        for (std::map<int,std::vector<std::string> >::const_reverse_iterator it = m_adm->rbegin();
             it != m_adm->rend(); ++it) {
            int val = it->first;
            if (val < adm_min) adm_min = val;
            if (val > adm_max) adm_max = val;
            const std::vector<std::string>& vec = it->second;
            for (int i = 0; i < (int)vec.size(); i++) {
                if (adm.find(vec[i]) == adm.end()) adm[vec[i]] = val;
            }
        }
    }

    if (m_tui && !m_tui->empty()) {
        for (std::map<int,std::vector<std::string> >::const_iterator it = m_tui->begin();
             it != m_tui->end(); ++it) {
            int val = it->first;
            if (val < tui_min) tui_min = val;
            if (val > tui_max) tui_max = val;
            const std::vector<std::string>& vec = it->second;
            for (int i = 0; i < (int)vec.size(); i++) {
                if (tui.find(vec[i]) == tui.end()) tui[vec[i]] = val;
            }
        }
    }

    if (m_siz && !m_siz->empty()) {
        for (std::map<int,std::vector<std::string> >::const_iterator it = m_siz->begin();
             it != m_siz->end(); ++it) {
            int val = it->first;
            if (val < siz_min) siz_min = val;
            if (val > siz_max) siz_max = val;
            const std::vector<std::string>& vec = it->second;
            for (int i = 0; i < (int)vec.size(); i++) {
                if (siz.find(vec[i]) == siz.end()) siz[vec[i]] = val;
            }
        }
    }

    if (sat_min > sat_max) { sat_min = 0; sat_max = 1; }
    if (adm_min > adm_max) { adm_min = 0; adm_max = 1; }
    if (tui_min > tui_max) { tui_min = 0; tui_max = 1; }
    if (siz_min > siz_max) { siz_min = 0; siz_max = 1; }
}

void hashmap_of_hashmaps::build_weighted_scores(std::vector<std::pair<float,std::string> >& out,
                                                float wTuition, float wAcceptance, float wSAT, float wUndergrad)
{
    out.clear();

    std::unordered_map<std::string,int> sat;
    std::unordered_map<std::string,int> adm;
    std::unordered_map<std::string,int> tui;
    std::unordered_map<std::string,int> siz;
    int sat_min, sat_max, adm_min, adm_max, tui_min, tui_max, siz_min, siz_max;

    collect_category_values(sat, adm, tui, siz, sat_min, sat_max, adm_min, adm_max, tui_min, tui_max, siz_min, siz_max);

    std::unordered_set<std::string> all_names;
    for (std::unordered_map<std::string,int>::const_iterator it = sat.begin(); it != sat.end(); ++it) all_names.insert(it->first);
    for (std::unordered_map<std::string,int>::const_iterator it = adm.begin(); it != adm.end(); ++it) all_names.insert(it->first);
    for (std::unordered_map<std::string,int>::const_iterator it = tui.begin(); it != tui.end(); ++it) all_names.insert(it->first);
    for (std::unordered_map<std::string,int>::const_iterator it = siz.begin(); it != siz.end(); ++it) all_names.insert(it->first);

    float denom = wTuition + wAcceptance + wSAT + wUndergrad;
    if (denom <= 0.0f) denom = 1.0f;

    for (std::unordered_set<std::string>::const_iterator it = all_names.begin();
         it != all_names.end(); ++it)
    {
        const std::string& name = *it;
        float score = 0.0f;

        std::unordered_map<std::string,int>::const_iterator jt;

        jt = sat.find(name);
        if (jt != sat.end()) {
            float range = (float)(sat_max - sat_min);
            if (range <= 0.0f) range = 1.0f;
            float norm = (float)(jt->second - sat_min) / range;
            score += norm * wSAT;
        }

        jt = adm.find(name);
        if (jt != adm.end()) {
            float range = (float)(adm_max - adm_min);
            if (range <= 0.0f) range = 1.0f;
            float norm = (float)(jt->second - adm_min) / range;
            score += norm * wAcceptance;
        }

        jt = tui.find(name);
        if (jt != tui.end()) {
            float range = (float)(tui_max - tui_min);
            if (range <= 0.0f) range = 1.0f;
            float norm = (float)(tui_max - jt->second) / range;
            score += norm * wTuition;
        }

        jt = siz.find(name);
        if (jt != siz.end()) {
            float range = (float)(siz_max - siz_min);
            if (range <= 0.0f) range = 1.0f;
            float norm = (float)(siz_max - jt->second) / range;
            score += norm * wUndergrad;
        }

        score = score / denom;
        out.push_back(std::make_pair(score, name));
    }
}

void hashmap_of_hashmaps::build_weighted_scores_filtered(std::vector<std::pair<float,std::string> >& out,
                                                         float wTuition, float wAcceptance, float wSAT, float wUndergrad,
                                                         const std::unordered_set<std::string>& allowed)
{
    out.clear();

    std::unordered_map<std::string,int> sat;
    std::unordered_map<std::string,int> adm;
    std::unordered_map<std::string,int> tui;
    std::unordered_map<std::string,int> siz;
    int sat_min, sat_max, adm_min, adm_max, tui_min, tui_max, siz_min, siz_max;

    collect_category_values(sat, adm, tui, siz, sat_min, sat_max, adm_min, adm_max, tui_min, tui_max, siz_min, siz_max);

    float denom = wTuition + wAcceptance + wSAT + wUndergrad;
    if (denom <= 0.0f) denom = 1.0f;

    for (std::unordered_set<std::string>::const_iterator it = allowed.begin(); it != allowed.end(); ++it) {
        const std::string& name = *it;
        float score = 0.0f;

        std::unordered_map<std::string,int>::const_iterator jt;

        jt = sat.find(name);
        if (jt != sat.end()) {
            float range = (float)(sat_max - sat_min);
            if (range <= 0.0f) range = 1.0f;
            float norm = (float)(jt->second - sat_min) / range;
            score += norm * wSAT;
        }

        jt = adm.find(name);
        if (jt != adm.end()) {
            float range = (float)(adm_max - adm_min);
            if (range <= 0.0f) range = 1.0f;
            float norm = (float)(jt->second - adm_min) / range;
            score += norm * wAcceptance;
        }

        jt = tui.find(name);
        if (jt != tui.end()) {
            float range = (float)(tui_max - tui_min);
            if (range <= 0.0f) range = 1.0f;
            float norm = (float)(tui_max - jt->second) / range;
            score += norm * wTuition;
        }

        jt = siz.find(name);
        if (jt != siz.end()) {
            float range = (float)(siz_max - siz_min);
            if (range <= 0.0f) range = 1.0f;
            float norm = (float)(siz_max - jt->second) / range;
            score += norm * wUndergrad;
        }

        score = score / denom;
        out.push_back(std::make_pair(score, name));
    }
}