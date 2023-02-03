#!/bin/bash
cd Fichier_externe
make all > /dev/null 2> $      # Execute mon makefile / "> /dev/null " permet que mon makefile n'affiche rien dans la console sauf les erreurs 
cd ..

option_obli=0       # Ce int sera le nombre d'option obligatoire demandé 
declare -a option_liste     # Ce tablau stockera toutes les options demandé par l'utilisateur 
lieu=0          # Ce int sera le nombre de lieu obligatoire demandé 
lelieu=""       # Cette chaîne permet de stocker la restriction géographique demandé 
affichage="--avl"       # Cette chaîne stock le type de tri demandé, est de base à AVL

for i in $* ;do     
    # parcours tout mes arguments pour voir si la commande --help a été demandé et je vérifie en même temps si j'ai bien l'option f 
    if [ "$i" == "--help" ] ;then 
        cat Fichier_externe/help.txt
        exit 1
    fi 
    if [ "$i" == "-f" ] ;then   # option f bien rentrée 
        option_obli=1   
    fi 
done

if [ ! $option_obli = 1 ] ;then # Si pas égale à 1 c'est qu'il n'y a pas l'option "f"
    echo "Pas de fichier"
    exit 1
fi
option_obli=0

while getopts "f:t:p:d:whmFGSAOQ-:" option; do # getops me permettra de regarder tout les arguments et de traiter ceux égaux à f:t:p:d:whmFGSAOQ-:" Les options avec : sont ceux où on attends un argument 
    case "$option" in 
    f)
        extension=$(echo $OPTARG | cut -d'.' -f2)   # je récupère uniquement l'extension de mon fichier
        fichier=$OPTARG     # "$OPTARG" correspond à ce qu'il y a après le -f. "$OPTARG" disponible grâce à "f:"
        if [ "$extension" != "csv" ];then   
            echo "Le fichier n'a pas la bonne extension"
            exit 1
        elif  [ ! -e $fichier ]; then
            echo "Le fichier n'existe pas"
            exit 1
        elif  [ ! -w $fichier ]; then
            echo "Le fichier ne peux pas être lu"
            exit 1          
        elif  [ ! -s $fichier ]; then
            echo "Le fichier est vide"
            exit 1
        fi
    ;;
    t)  
        if (( $OPTARG > 0 )) && (( $(($OPTARG/4)) < 1 )); then      # Je n'ai que 3 arguments possible pour T donc si $OPTARG / 4 est égaux ou supp à 1 c'est que j'ai trop un arguments trop haut
            if (( $OPTARG == 1 ));then 
                option_liste[$option_obli+1]="t1"   # J'ajoute mon option demandé a ma liste avec comme compteur otption_obli +1
                                                        # +1 car [0] est pour l'option d 
                option_obli=$(("$option_obli" + 1))     # J'ajoute 1 à mon compteur
            elif (( $OPTARG == 2 ));then 
                option_liste[$option_obli+1]="t2"
                option_obli=$(("$option_obli" + 1))
            else 
                option_liste[$option_obli+1]="t3"
                option_obli=$(("$option_obli" + 1))
            fi
        else 
            echo "Erreur : valeur pour temperature pas comprise dans [1;3] ";
            exit 1
        fi
        ;;
    p)
        if (( $OPTARG > 0 )) && (( $(($OPTARG/4)) < 1 )); then  #Je n'ai que 3 arguments possible pour P donc si $OPTARG / 4 est égaux ou supp à 1 c'est que j'ai trop un arguments trop haut
            if (( $OPTARG == 1 ));then 
                option_liste[$option_obli+1]="p1"   # J'ajoute mon option demandé a ma liste avec comme compteur otption_obli +1
                                                        # +1 car [0] est pour l'option d 
                option_obli=$(("$option_obli" + 1))     # J'ajoute 1 à mon compteur
            elif (( $OPTARG == 2 ));then 
                option_liste[$option_obli+1]="p2"
                option_obli=$(("$option_obli" + 1))
            else 
                option_liste[$option_obli+1]="p3"
                option_obli=$(("$option_obli" + 1))
            fi
        else
            echo "Erreur : valeur pour pression pas comprise dans [1;3] ";
            exit 1
        fi
        ;;
    d)                                          # L'option d correspond a un filtre temporel "AAAA-MM-JJ,AAAA-MM-JJ"
       min=$(echo $OPTARG|cut -d"," -f1)        # Je coupe la chaine en deux avec la virgule 
       max=$(echo $OPTARG|cut -d"," -f2)
       if [ -z "$min" ] || [ -z "$max" ] || [ -z "$(date -d "$min" +%s)" ] || [ -z "$(date -d "$max" +%s)" ]; then # Je vérifie si min et max existe et si ils sont bien en format de date et que la date est correcte 
           exit 1
       fi
       option_liste[0]="d"
    ;;
    w)
        option_liste[$option_obli+1]="w"         # J'ajoute mon option demandé a ma liste avec comme compteur otption_obli +1
                                                        # +1 car [0] est pour l'option d 
        option_obli=$(("$option_obli" + 1))     # J'ajoute 1 à mon compteur
        ;;
    h) 
        option_liste[$option_obli+1]="h"
        option_obli=$(("$option_obli" + 1))
        ;;
    m)
        option_liste[$option_obli+1]="m"
        option_obli=$(("$option_obli" + 1))
        ;;
    F)
        lelieu="F"                      # lelieu devient F car l'option demandé est F
        lieu=$(("$lieu" + 1))           # J'ajoute 1 à mon compteur
        ;;
    G) 
        lelieu="G"
        lieu=$(("$lieu" + 1))
        ;;
    S)
        lelieu="S"
        lieu=$(("$lieu" + 1))
        ;;
    A)
        lelieu="A"
        lieu=$(("$lieu" + 1))
        ;;
    O) 
        lelieu="O"
        lieu=$(("$lieu" + 1))
        ;;
    Q)
        lelieu="Q"
        lieu=$(("$lieu" + 1))
        ;;
    -)          # Pour les options longues je considère "-" comme une option course et le reste comme son argument 
        if [ $OPTARG == "abr" ]; then 
            affichage="--abr"       # affichage devient alors la syntax demandé par le fichier c  
        elif [ $OPTARG == "avl" ]; then 
            affichage="--avl"
        elif [ $OPTARG == "tab" ]; then 
            affichage="--tab"
        else    # sinon l'option longue rentré n'est pas valide 
            echo "Erreur dans l'option longue, seules options possible : abr / avl / tab"
            exit 1
        fi
    ;;
    *)
        echo "Option non valide "
        exit 1
    ;;
    esac 
    
done
if (( $option_obli < 1 )); then     # Voici à quoi sert le compteur aussi 
    echo "Vous n'avez pas mis d'option"
    exit 1
fi 
if (( $lieu > 1 )); then     # lieu ne peux être que 0 ( pas d'option géo ) ou 1 si on a une option géo
    echo "Vous avee mis trop de limitation géographique ( 1 max )"
    exit 1
fi 
if (( $lieu == 1 )); then # Si besoin de restriction géo alors 
    case $lelieu in     # je fais un case pour savoir de quelle réstriction il s'agit 
    F)
        awk -F";" '{
        split($10, coords, ",")     # je sépare la colonne des coordonées en deux dans un tableau 
        if(coords[1] + 0 >=41 && coords[1] + 0 <=52 && coords[2] + 0 >=-5 && coords[2] + 0 <=10 ){  # je fais un cadrillage avec les coordoneés gps / le "+0" me permet de gérer des doonées float 
            print $0    # Je récupère la ligne entière 
        }
        }' "$fichier" > meteo.csv   # meteo.txt est mon fichier après filtrage 
        ;;
    G) 
        awk -F";" '{        # Le procédé est le même pour le reste du case 
        split($10, coords, ",")
        if(coords[1] + 0 >=2 && coords[1] + 0 <=5 && coords[2] + 0 >=-55 && coords[2] + 0 <=-51 ){
            print $0
        }
        }' "$fichier"  > meteo.csv
        ;;
    S)
        awk -F";" '{
        split($10, coords, ",")
        if(coords[1] + 0 <=46.6 && coords[1] + 0 >=45.2 && coords[2] + 0 >=-56.5 && coords[2] + 0 <=-56 ){
            print $0
        }
        }' "$fichier"  > meteo.csv
        ;;
    A)
        awk -F";" '{
        split($10, coords, ",")
        if(coords[1] + 0 >=11.4 && coords[1] + 0 <=18.5 && coords[2] + 0 >=-63 && coords[2] + 0 <=-59 ){
            print $0
        }
        }' "$fichier"  > meteo.csv
        ;;
    O) 
        awk -F";" '{
        split($10, coords, ",")
        if(coords[1] + 0 >=-55 && coords[1] + 0 <=22 && coords[2] + 0 >=21 && coords[2] + 0 <=132 ){
            print $0
        }
        }' "$fichier"  > meteo.csv
        ;;
    Q)
        awk -F";" '{
        split($10, coords, ",")
        if(coords[1] + 0 >=-83 && coords[1] + 0 <=-55){
            print $0
        }
        }' "$fichier"  > meteo.csv
        ;;
    esac 
else 
    tail -n +2 meteo_filtered_data_v1.csv > meteo.csv   # sinon on retire simplement la premiere colone du fichier csv s

fi  

for i in "${option_liste[@]}"; do   # je m'attaque maintenant au option courtes
    case $i in 
        d) 
            cut -d ";" -f2 meteo.csv | cut -d "T" -f1 > tmp.csv     # Je récupère suelement la date, puis je récupère lannée le mois et le jours
            paste -d ";" tmp.csv meteo.csv | awk -F";" -v min="$min" -v max="$max"  '{  # je colle mon fichier avec l année le mois à mon fichier de base 
                if ($1 >= min && $1 <= max) print $0    # ce qui me permet de comparer les dates avec mes dates min et max récupérer plus haut 
            }' | cut -d ";" -f2- > tmp1.csv     # je ne garde que mon fichier de base pas avec la colonne rajouter 
            rm meteo.csv
            mv tmp1.csv meteo.csv   # pour une question de simplicité je garde le nmo meteo.csv
        ;;
        t1)  
            awk -F";" '{print $1 ";" $11  }' meteo.csv > tmp.csv    # je récupère le numéro de station et la température moyenne 
            touch result.txt    # je crée mon fichier d'arrivé pour mon filtrage 
            ./Fichier_externe/t1    # je filtre en c car bien plus simple   
            touch sortie.txt     # je crée mon fichier d'arrivé pour mon tri  
            ./Fichier_externe/main -f result.txt -o sortie.txt "$affichage"  # j'éffectue mon tri 
            gnuplot << EOF       # cette ligne me permet de faire mon gnuplot dans mon prorgamme 
            reset   
            set terminal jpeg size 1600,1200        # je configure la taille de mon image et son type 
            set autoscale noextend      # désactive l'ajustement automatique 
            set output "Graphique/carteTemperaturee1.jpeg"   # définie le nom du ficher de sortie 
            set style data lines        # définit le style de visualisation des données pour le graphique
            set title "temperature en fonctions des numéros de stations"    # défini le titre 
            set datafile separator " "      # Définit le séparateur des données 
            set xlabel 'Stations'           # Définit le nom des données en abscisse 
            set ylabel 'Temperature'        # Définit le nom des données en ordonnée  
            set xrange [ * : * ]            # définit la plage des valeurs de l'axe X pour le tracé graphique. Les astérisques signifient que la plage n'est pas définie
            set xtics rotate by 45 right    #Elle indique que les étiquettes doivent être inclinées de 45 degrés vers la droite
            Couleur = "#AAA7FF"             # Définit la couleur 
            plot "sortie.txt" u 0:2:3:xticlabels(1) with filledcurve fc rgb Couleur title "temperature min et max",'' using 0:4 smooth mcspline lw 2 title "temperature moyenne"
            quit
EOF
        ;;
        t2)
            awk -F";" '{
                sum[$2]+=$11    # Je fais un tableau sum qui adition pour chaque date heure sa temperature 
                count[$2]++ 
            } END { for (station in sum) {printf "%s %s 0 0 0\n", station, sum[station]/count[station]}}' meteo.csv > result.txt
            awk -F" " '{system("date -d \"" $1 "\" +%s")}' result.txt > result1.txt # je transforme les dates en secondes
            cut -d " " -f2- result.txt > result2.txt    # ne récupère que les degres 
            paste -d " " result1.txt result2.txt > result3.txt  # mets les temps en secondes et les tempartures dans le même fichier 
            sed 's/,/./g' result3.txt > result4.txt     # Pour le traitement nous changons les virgules en point
            touch sortie.txt        # Crée le fichier de sortie 
            ./Fichier_externe/main -f result4.txt -o sortie.txt "$affichage" 
            gnuplot << EOF
            reset
            set terminal jpeg size 1600,1200
            set autoscale noextend
            set output "Graphique/carteTemperature2.jpeg"
            set style data lines
            set title "temperature en fonctions des numéros de stations"
            set datafile separator " "
            set xlabel 'date/heure en secondes'
            set ylabel 'Temperature moyenne'
            set xrange [ * : * ]
            set yrange [ * : * ]
            set xtics rotate by 45 right
            plot "sortie.txt" using 1:2 with lines title "Moyenne des mesures"
            quit
EOF
        ;;
        t3)
            awk -F";" '{
                print $2 " " $1 " " $11 " 0 0"    # Récupère la date, station et la température 
            }'  meteo.csv > result.txt
            touch sortie.txt
            ./Fichier_externe/main -f result.txt -o sortie.txt "$affichage" -t3
        ;;
        p1)
            awk -F";" '{print $1 ";" $7  }' meteo.csv > tmp.csv     # Pour les p1,p2,p3 c'est le même traitement que les temperatures 
            touch result.txt
            ./Fichier_externe/t1
            touch sortie.txt
            ./Fichier_externe/main -f result.txt -o sortie.txt "$affichage"
            gnuplot << EOF 
            reset
            set terminal jpeg size 1600,1200
            set autoscale noextend
            set output "Graphique/cartePressioon1.jpeg"
            set style data lines
            set title "Pression en fonctions des numéros de stations"
            set datafile separator " "
            set xlabel 'Stations'
            set ylabel 'Pression'
            set xrange [ * : * ]
            set xtics rotate by 45 right
            Couleur = "#FFD0A7"
            plot "sortie.txt" u 0:2:3:xticlabels(1) with filledcurve fc rgb Couleur title "temperature min et max",'' using 0:4 smooth mcspline lw 2 title "temperature moyenne"
            quit
EOF
        ;;
        p2)
            awk -F";" '{
                sum[$2]+=$7
                count[$2]++ 
            } END { for (station in sum) {printf "%s %s 0 0 0\n", station, sum[station]/count[station]}}' meteo.csv > result.txt
            awk -F" " '{system("date -d \"" $1 "\" +%s")}' result.txt > result1.txt
            cut -d " " -f2- result.txt > result2.txt
            paste -d " " result1.txt result2.txt > result3.txt
            sed 's/,/./g' result3.txt > result4.txt

            touch sortie.txt
            ./Fichier_externe/main -f result4.txt -o sortie.txt "$affichage" 
            gnuplot << EOF
            reset
            set terminal jpeg size 1600,1200
            set autoscale noextend
            set output "Graphique/cartePression2.jpeg"
            set style data lines
            set title "pression en fonctions des numéros de stations"
            set datafile separator " "
            set xlabel 'date/heure en secondes'
            set ylabel 'Pression moyenne'
            set xrange [ * : * ]
            set yrange [ * : * ]
            set xtics rotate by 45 right
            plot "sortie.txt" using 1:2 with lines title "Moyenne des mesures"
            quit
EOF
        ;;
        p3)
            awk -F";" '{
                print $2 " " $1 " " $7 " 0 0"    
            }'  meteo.csv > result.txt
            touch sortie.txt
            ./Fichier_externe/main -f result.txt -o sortie.txt "$affichage" -t3
        ;;
        w)
            cut -d ";" -f 1,4,5,10 meteo.csv | awk -F ";" '$2 != "" && $3 != "" {print}' | awk -F ";" '     # Je garde les colonnes du numéro de station, intensité et orientation du vent et des coordoonées gps et ne garde que les lignes ou les données de vents ne sont pas nul
                 {
                    split($4, coords, ",")
                    latitude[$1]=coords[1]
                    longitude[$1]=coords[2]
                    station[$1] = $1
                    orientation[$1] += $2
                    vitesse[$1] += $3
                    count[$1] += 1
                }    
                END {
                    for(word in station){
                        if ( count[word] > 0){
                            print station[word] " " orientation[word]/count[word] " " vitesse[word]/count[word] " " longitude[word] " " latitude[word]
                        }
                    } 
                }' > result1.txt
            touch sortie.txt
            sed 's/,/./g' result1.txt > result.txt
            ./Fichier_externe/main -f result.txt -o sortie.txt "$affichage"
            gnuplot << EOF
            reset
            set terminal jpeg size 1600,1200
            set autoscale noextend
            set output "Graphique/carteVent.jpeg"
            set style data lines
            set title "Vecteur vent par stations"
            set datafile separator " "
            set xlabel 'Longitude'
            set ylabel 'Latitude'
            set xrange [ -180 : 180 ]
            set yrange [ -90 : 90 ]
            set xtics rotate by 45 right
            set isosam 31,31
            NO_ANIMATION = 1
            set palette defined (0 "white", 1 "blue", 2 "red")
            plot "sortie.txt" u 4:5:(3*column(3)):(sin(column(2))) with vectors     # J'exprime les vecteur avec 3*l'intensité pour que les flèches se voient plus et l'orientation avec la formul pour 
            quit
EOF
        ;;
        h) 
             awk -F";" '{ if ($6 > max[$1]) max[$1] = $14       #Je récupère l altitude, les coordonnées et le numéro de station 
                split($10, coords, ",")
                latitude[$1]=coords[1]
                longitude[$1]=coords[2]
            } END { for (station in max) print max[station] " " station " " latitude[station]" "longitude[station]" 0"}' meteo.csv > result.txt
            touch sortie.txt      
            ./Fichier_externe/main -f result.txt -o sortie.txt "$affichage"
            gnuplot << EOF
            reset
            set terminal jpeg size 1600,1200
            set autoscale noextend
            set output "Graphique/carteAltitude.jpeg"
            set title "Carte interpolée altitude"
            set xlabel "Longitude"
            set ylabel "Latitude"
            set zlabel "altitude"
            set datafile separator " " 
            set cbrange [*:871]         # Défini la hauteur de z manuelement 
            set palette defined (0 "white", 1 "blue", 2 "red")
            set xtics rotate by 45 right
            set view map scale 1 
            set dgrid3d 50,50       # Définit une grille en 3D avec 50 points dans la direction x et 50 points dans la direction y.
            set pm3d map interpolate 5,5 flush begin nohidden3d         # Définit les options de visualisation pour le graphique en utilisant "pm3d".  sont les suivantes :
                                                                            #"set pm3d map" définit le mode de tracé 3D en utilisant une carte de couleur.
                                                                            #"interpolate 5,5" définit la résolution de l'interpolation pour le tracé en utilisant une grille 5x5.
                                                                            #"flush begin" définit le début de la zone de tracé.
                                                                            #"nohidden3d" désactive la suppression des faces cachées pour le tracé 3D.
            splot "sortie.txt" using 4:3:1 with pm3d notitle
EOF
        ;;
        m)
            awk -F";" '{ if ($6 > max[$1]) max[$1] = $6 #Je récupère l humidité, les coordonnées et le numéro de station 
                split($10, coords, ",")
                latitude[$1]=coords[1]
                longitude[$1]=coords[2]
            } END { for (station in max) print max[station] " " station " " latitude[station]" "longitude[station]" 0"}' meteo.csv > result.txt
            touch sortie.txt       
            ./Fichier_externe/main -f result.txt -o sortie.txt "$affichage" -r
            gnuplot << EOF
            reset
            set terminal jpeg size 1600,1200
            set autoscale noextend
            set output "Graphique/carteHumidite.jpeg"
            set title "Carte interpolée Humiditée"
            set xlabel "Latitude"
            set ylabel "Longitude"
            set zlabel "Humidité"
            set datafile separator " " 
            set palette defined (0 "white", 1 "blue", 2 "red")          # définit les couleurs de ma carte 
            set view map scale 1 
            set dgrid3d 50,50
            set pm3d map interpolate 5,5 flush begin nohidden3d
            splot "sortie.txt" using 3:4:1 with pm3d notitle
EOF
        ;;
    esac 
done

# suppression de tout mes fichiers temporaires utilisé

if ls result*.* &> /dev/null; then
  rm result*.*
fi
if ls sortie.txt &> /dev/null; then
  rm sortie.txt
fi
if ls tm*.* &> /dev/null; then
  rm tm*.*
fi
if ls meteo.txt &> /dev/null; then
  rm meteo.txt
fi
if ls meteo.csv &> /dev/null; then
  rm meteo.csv
fi
cd Fichier_externe
make clean > /dev/null 2> terminal
cd ..