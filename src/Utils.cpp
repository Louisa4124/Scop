#include "../include/Utils.hpp"

void printHelp(const char *progName)
{
    std::cout << "\n******** Manuel d'utilisation de Scop *********\n\n";
    std::cout << "Utilisation : \n" << progName << " <fichier.obj> [texture.bmp]\n";
    std::cout << "PS: la texture est optionnel \n\n";
    std::cout << "Commandes disponibles :\n";
    std::cout << "- w, a, s, d, q, e : Déplacer le modèle sur les axes x, y et z\n";
    std::cout << "- ↑, ↓, →, ←, 0, . : Tourner le modèle manuellement\n";
    std::cout << "- 1, 2, 3 : Activer les rotations constantes du modèle\n";
    std::cout << "- c : Changer le mode de couleur\n";
    std::cout << "- t : Activer / désactiver la texture\n";
    std::cout << "- m : Changer le mode d'affichage (fil de fer, remplissage, points)\n";
    std::cout << "- +, - : Augmenter / réduire la vitesse de rotation\n";
    std::cout << "- Esc : Fermer la fenêtre\n";
}

bool checkExtension(const std::string &fullString, const std::string &ending)
{
    if (fullString.length() >= ending.length())
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    return false;
}

std::string findDefaultTexture(const std::string &objPath)
{
    size_t lastSlash = objPath.find_last_of("/\\");
    std::string filename = (lastSlash == std::string::npos) ? objPath : objPath.substr(lastSlash + 1);
    size_t lastDot = filename.find_last_of('.');
    std::string baseName = (lastDot == std::string::npos) ? filename : filename.substr(0, lastDot);

    std::string matchingPath = "assets/" + baseName + ".bmp";
    std::ifstream matchingFile(matchingPath);
    if (matchingFile.is_open())
        return matchingPath;

    //  par defaut si on ne trouve aucune texture on met les licornes
    std::ifstream defaultTex("assets/mlp.bmp");
    if (defaultTex.is_open())
        return "assets/mlp.bmp";

    return "";
}