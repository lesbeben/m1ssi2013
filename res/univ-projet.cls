% univ-doc.cls
% Cette classe latex doit soit se trouver dans le repertoire
% courant ainsi que le logo soit être dans votre path latex
\ProvidesClass{univ-projet}

% Utilisation de la classe article comme base
\LoadClass[10pt]{article}

% utilisation des convention française
\usepackage[francais]{babel}

%Macro outil, ne devrait pas être utiliser en dehors de ce fichier.
\def\addtomacro#1#2{\edef#1{\unexpanded\expandafter{#1#2}}}

% champs supplmentaires pour la page de garde:
% projet, filiere, version, relecteur et logo.
% La command \histentry{version}{date}{description}
% permet d'ajouter des entrees aux tableau sur la page de garde.
\makeatletter
    \newcommand{\projet}[1]{\def \@projet {#1}}
    \newcommand{\projdesc}[1]{\def \@projdesc {#1}}
    \newcommand{\filiere}[1]{\def \@filiere {#1}}
    \newcommand{\version}[1]{\def \@version {#1}}
    \newcommand{\relecteur}[1]{\def \@relecteur {#1}}
    \newcommand{\signataire}[1]{\def \@signataire {#1}}
    \newcommand{\logo}[1]{\def \@logo {#1}}
    \newcommand{\histentry}[3]{\addtomacro{\@history}{#1&#2&#3\\\hline}}
    \projet{}
    \filiere{}
    \version{}
    \relecteur{}
    \logo{}
    \signataire{}
    \def \@history {} % Initialise l'historique a vide.
\makeatother

% marges
\usepackage[top=1in,bottom=1in,left=1in,right=1in]{geometry}

% indentations
\usepackage{indentfirst}
\setlength{\parindent}{18pt}
\setlength{\parskip}{3pt}

% espacements
\usepackage{setspace}

% gestion des references
\usepackage{varioref}
\usepackage{hyperref}

% gestion des figures
\usepackage{graphicx}

% gestion des couleurs
\usepackage[table]{xcolor}

% gestion bibliography
\usepackage{natbib}

% entête et pagination
\makeatletter
    \usepackage{fancyhdr}
    \pagestyle{fancy}
    \setlength{\headheight}{14pt}
    \renewcommand{\footrulewidth}{\headrulewidth}
    \fancyhf{}

    \fancyhead[L]{\@filiere}
    \fancyhead[C]{\large \@projet}
    \fancyhead[R]{\@title-\@version}

    \fancyfoot[L]{\@author}
    \fancyfoot[R]{\thepage}
\makeatother

% Permet de conditionner la mise en page de la page de garde.
\usepackage{ifthen}

% Mecanique de fiche.
\newcommand{\fiche}[9] {
	\noindent
\begin{tabular}{|p{3.5cm}| p{1cm} | p{3cm} | p{.5cm} | p{7cm}|} 
\hline
\multicolumn{2}{|>{\color{white}\columncolor{blue}}l|}{\bfseries{ Nom}} & \multicolumn{3}{>{\color{white}\columncolor{blue}}l|}{\bfseries{#1}}\\
\hline
\multicolumn{2}{|l|}{\bfseries{\bfseries{Acteurs concernés}}} & \multicolumn{3}{m{10.5cm}|}{#2}\\
\hline
\multicolumn{2}{|l|}{\bfseries{\bfseries{Description}}} & \multicolumn{3}{m{10.5cm}|}{#3}\\
\hline
\multicolumn{2}{|l|}{\bfseries{\bfseries{Préconditions}}} & \multicolumn{3}{m{10.5cm}|}{#4}\\
\hline
\multicolumn{2}{|l|}{\bfseries{\bfseries{Evénements déclenchants}}} & \multicolumn{3}{m{10.5cm}|}{#5}\\
\hline
\multicolumn{2}{|l|}{\bfseries{\bfseries{Conditions d'arrêt}}} & \multicolumn{3}{m{10.5cm}|}{#6}\\
\hline
\multicolumn{5}{|>{\columncolor[gray]{.7}}l|}{\bfseries{\bfseries{Description du flot d'événements principal:}}}\\
\hline
\multicolumn{3}{|>{\columncolor[gray]{.7}}c|}{\bfseries{\bfseries{Acteur(s)}}} & \multicolumn{2}{>{\columncolor[gray]{.7}}c|}{\bfseries{\bfseries{Système}}}\\
\hline
\multicolumn{3}{|p{7.5cm}|}{#7} & \multicolumn{2}{p{7.5cm}|}{#8}\\
\hline
\bfseries{Flots d'exceptions:} & \multicolumn{4}{|p{11.5cm}|}{#9}\\
\hline
\end{tabular}
\\
}

% page de garde
\makeatletter
\def \maketitle{%
    \begin{titlepage}
        \ifx \@logo \@empty
            \ifx \@projet \@empty
                \relax
            \else
                    \flushright
                    {\Huge {\bfseries \@projet}}\newline
                    \ifx \@projdesc \@empty
                        \relax
                    \else
                        {\Large \@projdesc} 
                    \fi
            \fi
        \else
            \includegraphics[width=0.33\textwidth]{\@logo}
            \ifx \@projet \@empty
                \relax
            \else
                \parbox[b][4em][b]{0.66\textwidth}{
                    \flushright
                    {\Huge {\bfseries \@projet}}\newline
                    \ifx \@projdesc \@empty
                        \relax
                    \else
                        {\Large \@projdesc} 
                    \fi
                }
            \fi
        \fi
        
        ~\vfill
        \begin{center}
            \ifx \@title \@empty
                \relax
            \else
                {\huge \textbf{\@title} {\Large \@version}}\\
            \fi
            {\@date}\\
            \vspace{2em}
            \ifx \@author \@empty
                \relax
            \else
                Auteur: \@author\\
            \fi
            \ifx \@relecteur \@empty
                \relax
            \else
                Relecteur: \@relecteur\\
            \fi
            \ifx \@signataire \@empty    
                \relax
            \else
                Approuv\'e par: \@signataire\\
            \fi
            \vspace{5em}
            \ifx \@history \@empty
                \relax
            \else
                \begin{tabular}[t]{|c|c|p{10cm}|}
                    \hline
                    \rowcolor{gray} \textcolor{white} {\bfseries Version} & \textcolor{white} {\bfseries Date} & \textcolor{white} {\bfseries Changelog} \\
                    \hline
                    \@history
                    &&\\
                    \hline 
                \end{tabular}
            \fi
        \end{center}
        \vfill
    \end{titlepage}
}
\makeatother
