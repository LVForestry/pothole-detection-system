import os
import sys
import subprocess
from datetime import datetime, timezone, timedelta
import pytz

# Fuseau Riga (EET/EEST)
riga_tz = pytz.timezone('Europe/Riga')

# Distance fixe entre caméra (avant) et capteur (arrière) en mètres
DISTANCE_METRES = 15.0

# Durée des vidéos Garmin en minutes (pour matching)
VIDEO_DURATION_MIN = 1

# Décalage entre création du fichier et début de l'enregistrement (secondes)
VIDEO_OFFSET_SEC = 37

# Dossier pour les thumbnails
VIDEO_THUMBNAILS_DIR = "thumbnails"

# Largeur estimée de l'image pour espacement des textes
IMAGE_WIDTH = 2560

# Liste des capteurs
SENSORS = ['L1', 'L2', 'L3', 'L4', 'L5', 'L6', 'L7', 'L8', 'R8', 'R7', 'R6', 'R5', 'R4', 'R3', 'R2', 'R1']

# Chemins hardcodés (change-les ici si besoin)
LOGS_FILE = r"H:\301025.txt"
VIDEOS_DIR = r"H:\DCIM\105UNSVD"
OUTPUT_FILE = r"H:\logs_with_videos.html"

# Contenu SVG nettoyé (Silavav5.svg, defs supprimés)
SVG_CONTENT = '''<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg
   width="300"
   height="70"
   viewBox="0 0 74 18"
   version="1.1"
   id="svg5"
   xmlns="http://www.w3.org/2000/svg"
   xmlns:svg="http://www.w3.org/2000/svg">
  <g
     inkscape:label="Calque 1"
     inkscape:groupmode="layer"
     id="layer1">
    <g
       id="g26465"
       transform="matrix(0.26458333,0,0,0.26458333,-105.11321,3.4960976)">
      <g
         id="g37740"
         transform="translate(117.52012,108.97679)">
        <g
           clip-path="url(#clipTrailer)"
           id="g37646" />
        <g
           inkscape:label="Calque 1"
           id="layer1-1"
           transform="matrix(3.7795276,0,0,3.7795276,124.41548,23.696652)">
          <path
             style="fill:#000000;stroke-width:0.0391863"
             d="m 42.688589,-21.47874 c -0.45782,-0.0732 -0.67868,-0.25807 -0.67868,-0.56819 v -0.15764 h 0.35472 c 0.34441,0 0.35545,0.003 0.37957,0.0955 0.0341,0.13114 0.21695,0.2124 0.4779,0.2124 0.27201,0 0.40854,-0.048 0.44409,-0.15605 0.0454,-0.1381 -0.0597,-0.19079 -0.65459,-0.32807 -0.63207,-0.14586 -0.74765,-0.19277 -0.8972,-0.36427 -0.0859,-0.0985 -0.10446,-0.15864 -0.10434,-0.33818 1.5e-4,-0.27705 0.0935,-0.43091 0.32769,-0.54029 0.2615,-0.12214 0.60117,-0.17032 1.05006,-0.14893 0.63467,0.0302 1.01573,0.23686 1.01573,0.55078 v 0.13177 h -0.35635 c -0.32651,0 -0.35858,-0.007 -0.38284,-0.0805 -0.0715,-0.21724 -0.74953,-0.25929 -0.83213,-0.0516 -0.0452,0.11353 0.0879,0.2011 0.40837,0.26876 0.87833,0.18546 1.11418,0.29567 1.22343,0.57169 0.12637,0.31927 -0.004,0.60805 -0.34633,0.76776 -0.35222,0.16427 -0.91403,0.21736 -1.4291,0.13503 z m 1.91442,-1.12999 v -1.13551 h 0.41889 0.41888 v 1.13551 1.13549 h -0.41888 -0.41889 z m 1.19682,0 v -1.13551 h 0.39819 0.39819 l 0.0107,0.87569 0.0107,0.87567 0.39893,0.0108 0.39894,0.0108 0.49868,-0.88647 0.49867,-0.88647 0.45914,-8e-5 0.45914,-7e-5 0.22963,0.43303 c 0.12629,0.23816 0.39029,0.73615 0.58665,1.10663 0.19637,0.37048 0.35767,0.68716 0.35845,0.70373 7.2e-4,0.0166 -0.19859,0.0253 -0.44304,0.0193 l -0.44446,-0.0108 -0.0795,-0.20208 -0.0795,-0.20208 h -0.6551 -0.6551 l -0.097,0.21169 -0.097,0.21171 h -1.07763 -1.07764 z m 3.24621,-0.16359 c -0.099,-0.20642 -0.19312,-0.38829 -0.20911,-0.40415 -0.016,-0.0158 -0.12103,0.153 -0.23341,0.37529 l -0.20432,0.40414 h 0.41343 0.41344 z m 1.64603,1.05851 c -0.214,-0.4626 -0.64387,-1.3789 -0.78802,-1.67971 -0.0851,-0.17758 -0.15473,-0.33839 -0.15473,-0.35734 0,-0.019 0.19057,-0.0296 0.42349,-0.0236 l 0.42349,0.0109 0.15763,0.34642 c 0.0867,0.19054 0.21156,0.46767 0.27746,0.61587 0.23575,0.53012 0.28781,0.63511 0.31496,0.63511 0.0152,0 0.0647,-0.0909 0.11013,-0.20208 0.0742,-0.18165 0.24226,-0.55333 0.53941,-1.19324 l 0.10277,-0.22132 h 0.35459 c 0.19503,0 0.36441,0.009 0.37641,0.021 0.012,0.0116 -0.10267,0.28439 -0.25482,0.60625 -0.15215,0.32185 -0.32581,0.68911 -0.38591,0.81614 -0.0601,0.12702 -0.17133,0.36459 -0.24718,0.52793 l -0.1379,0.29699 -0.50915,0.002 -0.50915,0.002 z m 1.33119,0.18886 c 0,-0.0147 0.91402,-1.64555 1.12518,-2.00758 l 0.13471,-0.23095 0.42129,-0.0108 c 0.30559,-0.008 0.43292,0.003 0.46365,0.0385 0.0694,0.0809 1.16636,2.13038 1.16636,2.1792 0,0.0315 -0.12884,0.0449 -0.43005,0.0449 h -0.43005 l -0.0774,-0.19247 -0.0774,-0.19245 h -0.66259 -0.6626 l -0.0934,0.19245 -0.0934,0.19247 h -0.39211 c -0.21567,0 -0.39212,-0.006 -0.39212,-0.0132 z m 2.03459,-0.90576 c 0,-0.0185 -0.0815,-0.20316 -0.18116,-0.41028 -0.11273,-0.23435 -0.19529,-0.36297 -0.21856,-0.34052 -0.0358,0.0345 -0.39816,0.7334 -0.39816,0.76786 0,0.009 0.17952,0.0166 0.39894,0.0166 0.21942,0 0.39894,-0.0151 0.39894,-0.0336 z m -5.48384,-1.9334 c -0.78255,-0.62227 -1.69615,-1.04295 -2.63455,-1.21312 -0.53292,-0.0966 -1.64291,-0.0969 -2.2939,-5.3e-4 -0.38313,0.0567 -1.30962,0.24466 -1.56583,0.3177 -0.0663,0.019 -0.0698,-0.30131 -0.0698,-6.34597 v -6.365879 l 0.24933,0.0286 c 0.13714,0.0157 0.61736,0.0474 1.06716,0.0705 1.49794,0.0768 2.13074,0.12144 2.53326,0.17878 1.16634,0.16615 1.81838,0.442029 2.44218,1.033269 l 0.3278,0.31069 0.20111,-0.2595 c 0.56659,-0.731069 1.54772,-1.122409 3.15261,-1.257459 0.25233,-0.0212 0.80885,-0.0482 1.23671,-0.0599 0.42786,-0.0118 1.07863,-0.0309 1.44615,-0.0425 l 0.66823,-0.0212 v 6.353409 c 0,5.7278 -0.006,6.35341 -0.0622,6.35341 -0.0342,0 -0.27207,-0.0511 -0.5286,-0.11333 -0.55708,-0.13535 -1.22288,-0.24947 -1.82281,-0.31244 -0.85649,-0.0899 -1.9217,0.0339 -2.54389,0.2957 -0.53068,0.22327 -1.06819,0.63314 -1.46557,1.11754 l -0.0996,0.12142 z m 0.53272,-0.995 c 1.13575,-1.08265 2.29591,-1.40362 4.7317,-1.30907 0.48272,0.0187 0.91806,0.0348 0.96743,0.0356 l 0.0898,0.002 v -5.3888 -5.388809 l -0.1895,-7.7e-4 c -0.10422,-5.4e-4 -0.62035,-0.0171 -1.14694,-0.0365 -1.04282,-0.0384 -1.93166,-0.003 -2.42928,0.0959 -0.82472,0.164369 -1.65949,0.550559 -2.3074,1.067489 l -0.18877,0.15063 -0.32985,-0.19104 c -1.04915,-0.60764 -2.12148,-0.93697 -3.41269,-1.048099 -0.25724,-0.0222 -0.94343,-0.0352 -1.52489,-0.029 l -1.05718,0.0112 v 5.369569 c 0,5.34793 3e-4,5.36952 0.0798,5.35747 0.0439,-0.007 0.42985,-0.0215 0.85772,-0.0329 0.88189,-0.0237 1.40989,0.0286 2.19415,0.21743 0.96342,0.23189 2.05188,0.68728 3.03193,1.26851 0.14263,0.0846 0.27136,0.1544 0.28608,0.15515 0.0147,5.4e-4 0.17129,-0.13642 0.34794,-0.30481 z m -0.84649,-0.59126 c -0.46016,-0.26934 -0.4712,-0.279 -0.44779,-0.39183 0.0863,-0.41603 0.22127,-2.50286 0.22028,-3.40671 l -7.2e-4,-0.71209 -0.69717,-0.71944 c -0.38345,-0.39571 -0.69679,-0.74213 -0.6963,-0.76983 4.7e-4,-0.0277 0.0331,-0.0908 0.0725,-0.14018 l 0.0716,-0.0898 0.28037,0.28225 c 0.1542,0.15523 0.50138,0.5111 0.77152,0.79082 0.27013,0.27971 0.5037,0.50109 0.51905,0.49194 0.0153,-0.009 0.0402,-0.40257 0.0553,-0.87425 0.0381,-1.19174 0.0373,-1.18755 0.22752,-1.29315 l 0.15557,-0.0864 0.005,0.58814 c 0.002,0.32348 0.0161,0.81641 0.0299,1.09538 l 0.0252,0.50723 0.55262,-0.42556 0.55263,-0.42557 0.11935,0.11516 0.11935,0.11515 -0.27304,0.21437 c -0.15017,0.1179 -0.36699,0.285 -0.48183,0.37133 -0.3403,0.25581 -0.34011,0.25526 -0.29208,0.89324 0.0229,0.30381 0.0599,0.82457 0.0824,1.15723 0.0224,0.33267 0.0851,0.94756 0.13935,1.36645 0.14555,1.12477 0.16338,1.02408 -0.2375,1.3408 -0.18541,0.14649 -0.35147,0.26974 -0.36902,0.2739 -0.0175,0.005 -0.2442,-0.11671 -0.50368,-0.2686 z m -1.5204,-1.61171 c -0.0496,-0.14842 -0.17896,-1.08838 -0.15286,-1.11034 0.0169,-0.0142 0.22204,-0.12374 0.45594,-0.24344 l 0.42526,-0.21765 0.16015,0.46191 0.16016,0.46193 -0.46574,0.32089 c -0.57256,0.39449 -0.56222,0.38869 -0.58291,0.3267 z m 3.11904,-0.4511 c -0.25789,-0.16443 -0.47888,-0.30862 -0.49109,-0.3204 -0.0359,-0.0346 0.23389,-0.9151 0.27982,-0.91338 0.0413,0.002 0.69233,0.28917 0.83173,0.36747 0.0691,0.0388 0.0691,0.0613 8e-5,0.60547 -0.0395,0.31136 -0.0897,0.56396 -0.11167,0.5625 -0.022,-0.002 -0.25098,-0.1372 -0.50887,-0.30166 z m -4.59073,-0.37547 c -0.008,-0.008 0.0368,-0.22639 0.10018,-0.48532 0.0634,-0.25893 0.12323,-0.50705 0.13295,-0.55139 0.0158,-0.072 0.0713,-0.0878 0.5197,-0.1483 0.27611,-0.0372 0.5115,-0.0586 0.5231,-0.0473 0.0116,0.0112 0.008,0.22855 -0.008,0.48303 l -0.0286,0.46269 -0.34407,0.0784 c -0.18924,0.0431 -0.4648,0.11088 -0.61235,0.1506 -0.14755,0.0397 -0.27506,0.0657 -0.28335,0.0577 z m 6.17665,-0.34634 c -0.13165,-0.0309 -0.37287,-0.0861 -0.53606,-0.12259 -0.34603,-0.0774 -0.33762,-0.0634 -0.38241,-0.63977 l -0.0285,-0.36704 0.15435,5.5e-4 c 0.19472,5.4e-4 0.85641,0.0673 0.87801,0.0881 0.0137,0.0132 0.29363,1.0538 0.29363,1.09149 0,0.0242 -0.13868,0.005 -0.379,-0.0508 z m -6.6731,-0.92999 c -0.2912,-0.0329 -0.52409,-0.0752 -0.51754,-0.0942 0.038,-0.11002 0.65766,-0.91223 0.70509,-0.91281 0.0648,-5.5e-4 0.93194,0.19415 0.95605,0.21494 0.009,0.008 -0.0798,0.2045 -0.19663,0.43784 -0.19163,0.38286 -0.22236,0.42376 -0.31493,0.41909 -0.0564,-0.003 -0.34084,-0.032 -0.63204,-0.0649 z m 6.4772,-0.6613 c -0.1254,-0.23288 -0.21984,-0.42341 -0.20985,-0.42341 0.01,0 0.22557,-0.052 0.47909,-0.11547 0.25352,-0.0635 0.47814,-0.11548 0.49916,-0.11548 0.021,0 0.19271,0.21174 0.38154,0.47053 0.29103,0.39886 0.33263,0.4743 0.2731,0.49533 -0.0599,0.0212 -0.98594,0.11191 -1.14202,0.11191 -0.0292,0 -0.15562,-0.19054 -0.28102,-0.42341 z m -6.8533,-0.7074 c -0.27863,-0.15215 -0.51734,-0.28698 -0.53046,-0.29964 -0.021,-0.0203 0.90628,-0.58907 1.007,-0.61773 0.0457,-0.0131 0.85862,0.54949 0.83171,0.57546 -0.0857,0.0827 -0.76627,0.62379 -0.78169,0.6215 -0.011,-0.002 -0.24792,-0.12745 -0.52656,-0.27959 z m 7.2201,-0.32256 c -0.21319,-0.14273 -0.38795,-0.27683 -0.38837,-0.298 -4.6e-4,-0.0212 0.16361,-0.17562 0.3645,-0.34321 l 0.36524,-0.30472 0.53472,0.27043 c 0.29409,0.14873 0.53418,0.28585 0.53354,0.30471 -10e-4,0.0343 -0.9425,0.6312 -0.99448,0.6306 -0.0151,-1.6e-4 -0.20196,-0.11709 -0.41515,-0.25981 z m -7.12477,-1.1285 c -0.20426,-0.25697 -0.36583,-0.47129 -0.35904,-0.47627 0.007,-0.005 0.28623,-0.0521 0.62098,-0.10487 l 0.60863,-0.0958 0.23911,0.3947 c 0.13151,0.21709 0.23911,0.40991 0.23911,0.42848 0,0.0266 -0.54714,0.20861 -0.91756,0.30538 -0.0389,0.0101 -0.18986,-0.14794 -0.43123,-0.45159 z m 6.84581,0.0253 c -0.19108,-0.0247 -0.35995,-0.057 -0.37525,-0.0717 -0.0153,-0.0147 0.049,-0.22682 0.14287,-0.47124 l 0.17068,-0.4444 0.62345,0.0258 c 0.34289,0.0142 0.62606,0.0278 0.62925,0.0302 0.003,0.003 -0.11965,0.22523 -0.273,0.49516 l -0.2788,0.49077 -0.14589,-0.005 c -0.0802,-0.003 -0.30222,-0.025 -0.49331,-0.0498 z m -5.65342,-0.75106 -0.29604,-0.022 -0.17714,-0.5314 c -0.0974,-0.29227 -0.16668,-0.5415 -0.15388,-0.55385 0.0203,-0.0196 1.18398,0.17052 1.21351,0.1982 0.01,0.009 0.16836,0.88366 0.16836,0.92809 0,0.02 -0.34572,0.0112 -0.75481,-0.0191 z m 4.56806,-0.48249 c 0.015,-0.1701 0.0384,-0.38563 0.0519,-0.47895 l 0.0245,-0.16969 0.57846,-0.12757 c 0.31815,-0.0702 0.59963,-0.12804 0.62551,-0.12862 0.033,-5.4e-4 0.0238,0.0766 -0.0308,0.25877 -0.0428,0.1429 -0.10493,0.37239 -0.13797,0.51 -0.033,0.13761 -0.0767,0.27591 -0.097,0.30733 -0.0269,0.0417 -0.17284,0.0681 -0.5394,0.0976 l -0.50251,0.0405 z m -3.43426,-0.25889 c -0.24136,-0.10415 -0.45231,-0.20227 -0.46878,-0.21804 -0.0165,-0.0157 10e-4,-0.2836 0.0392,-0.59516 0.0571,-0.46727 0.0795,-0.56265 0.12821,-0.54462 0.1133,0.0419 0.99949,0.60867 0.99949,0.63917 0,0.0287 -0.24358,0.89401 -0.25437,0.90362 -0.003,0.003 -0.20242,-0.0808 -0.44377,-0.18497 z m 2.50049,0.0188 c -0.0207,-0.0381 -0.10381,-0.24057 -0.18474,-0.45003 -0.12064,-0.31225 -0.13766,-0.38912 -0.0945,-0.42687 0.029,-0.0253 0.24748,-0.18988 0.48559,-0.36569 0.29813,-0.22012 0.4422,-0.30515 0.46274,-0.27309 0.0315,0.0492 0.17996,0.68553 0.22071,0.94609 l 0.0241,0.15397 -0.23112,0.12472 c -0.12712,0.0686 -0.32429,0.17768 -0.43816,0.24241 -0.19247,0.10941 -0.20966,0.11282 -0.24462,0.0485 z m -1.37432,-0.38752 c -0.19153,-0.18565 -0.34824,-0.35093 -0.34824,-0.36729 0,-0.0335 0.51187,-0.95264 0.55694,-1.00009 0.027,-0.0284 0.70596,0.80587 0.72914,0.89595 0.0103,0.0399 -0.52812,0.80411 -0.56882,0.80735 -0.0114,5.3e-4 -0.17749,-0.15026 -0.36902,-0.33592 z"
             id="path39" />
          <text
             xml:space="preserve"
             transform="matrix(0.10247409,0,0,0.09856331,40.318269,-129.77425)"
             id="text1283"
             style="font-style:normal;font-weight:normal;font-size:40px;line-height:1.25;font-family:sans-serif;white-space:pre;shape-inside:url(#rect1285-5);fill:#000000;fill-opacity:1;stroke:none"><tspan
               x="220.18164"
               y="1029.335"
               id="tspan104">Pētniecības pieteikums 
</tspan><tspan
               x="220.18164"
               y="1079.335"
               id="tspan106">Nr 1.1.1.9/LZP/1/24/107</tspan></text>
          <text
             xml:space="preserve"
             transform="scale(0.26458333)"
             id="text31813-2"
             style="font-style:normal;font-weight:normal;font-size:40px;line-height:1.25;font-family:sans-serif;white-space:pre;shape-inside:url(#rect31815-1);fill:#000000;fill-opacity:1;stroke:none" />
        </g>
      </g>
    </g>
    <text
       xml:space="preserve"
       transform="scale(0.26458333)"
       id="text31813"
       style="font-style:normal;font-weight:normal;font-size:40px;line-height:1.25;font-family:sans-serif;white-space:pre;shape-inside:url(#rect31815);fill:#000000;fill-opacity:1;stroke:none" />
  </g>
</svg>'''

def parse_log_line(line):
    # Format attendu : CSV "name,date,time,lat,lng,speed,heading"
    # Ex. : L1,27-10-25,10:30:45,56.1234567,24.1234567,5.5,180.0
    parts = line.strip().split(',')
    if len(parts) < 7:
        return None, None, None, None
    name, date_str, time_str, lat_str, lng_str, speed_str, heading_str = parts[:7]
    try:
        # Date : DD-MM-YY (assume 20YY)
        day, month, year = map(int, date_str.split('-'))
        year += 2000 if year < 100 else 0  # 25 -> 2025
        # Time : HH:MM:SS
        hour, minute, second = map(int, time_str.split(':'))
        # UTC datetime
        utc_dt = datetime(year, month, day, hour, minute, second, tzinfo=timezone.utc)
        
        # Calcul du décalage : delta_t = distance / speed (en secondes)
        speed = float(speed_str)
        delta_t_seconds = DISTANCE_METRES / speed
        utc_dt_adjusted = utc_dt - timedelta(seconds=delta_t_seconds)
        
        # Convert to Riga local time
        local_dt = utc_dt_adjusted.astimezone(riga_tz)
        return local_dt, name, speed, line.strip()  # Retourne local_dt (heure caméra), name, speed, full_line
    except (ValueError, IndexError):
        return None, None, None, None

def get_video_timestamp(filename):
    # Utilise date de création du fichier (ctime), ajustée pour le début réel
    try:
        ctime = os.path.getctime(filename)
        dt = datetime.fromtimestamp(ctime, tz=timezone.utc)
        # Soustraire le décalage pour obtenir le début de l'enregistrement
        dt = dt - timedelta(seconds=VIDEO_OFFSET_SEC)
        return dt.astimezone(riga_tz)
    except OSError:
        return None

def find_closest_video(camera_dt, videos):
    # Trouve la vidéo qui contient l'heure caméra (vid_dt <= camera_dt <= vid_dt + duration)
    duration = timedelta(minutes=VIDEO_DURATION_MIN)
    for vid_dt, vid_file in videos:
        if vid_dt <= camera_dt <= vid_dt + duration:
            return vid_file
    # Si aucune ne contient, trouve la plus proche après (fallback)
    closest = None
    min_diff = timedelta(minutes=30)
    for vid_dt, vid_file in videos:
        diff = vid_dt - camera_dt
        if timedelta(0) <= diff <= min_diff:
            min_diff = diff
            closest = vid_file
    return closest

def extract_video_frame(video_path, offset_sec, output_path, triggered_sensor=""):
    # Extrait un frame avec FFmpeg et ajoute la liste des capteurs avec highlight
    if offset_sec < 0:
        return False
    vf_filters = []
    # Occuper 1/3 de la largeur, centré
    text_width = IMAGE_WIDTH // 3
    start_x = (IMAGE_WIDTH - text_width) // 2
    spacing = text_width // (len(SENSORS) - 1) if len(SENSORS) > 1 else 0
    for i, sensor in enumerate(SENSORS):
        color = "red" if sensor == triggered_sensor else "green"
        x_pos = start_x + i * spacing
        vf_filters.append(f"drawtext=text='{sensor}':x={x_pos}:y=10:fontsize=36:fontcolor={color}:box=1:boxcolor=black@0.5")
    vf_filter = ",".join(vf_filters)
    cmd = [
        'ffmpeg', '-y', '-i', video_path, '-ss', str(offset_sec),
        '-frames:v', '1', '-q:v', '2', '-vf', vf_filter, output_path
    ]
    try:
        result = subprocess.run(cmd, capture_output=True, timeout=30)
        return result.returncode == 0
    except (subprocess.TimeoutExpired, FileNotFoundError):
        return False

def main():
    # Utilise les variables globales
    logs_file = LOGS_FILE
    videos_dir = VIDEOS_DIR
    output_file = OUTPUT_FILE
    thumbnails_dir = VIDEO_THUMBNAILS_DIR
    
    # Crée le dossier thumbnails si besoin
    if not os.path.isdir(thumbnails_dir):
        os.makedirs(thumbnails_dir)
    
    # Vérifications de base
    if not os.path.isfile(logs_file):
        print(f"Erreur: Fichier logs '{logs_file}' introuvable")
        sys.exit(1)
    if not os.path.isdir(videos_dir):
        print(f"Erreur: Dossier vidéos '{videos_dir}' introuvable")
        sys.exit(1)
    
    # Lis les vidéos et extrait timestamps des métadonnées
    videos = []
    for f in os.listdir(videos_dir):
        if f.lower().endswith('.mp4'):  # Filtre MP4
            full_path = os.path.join(videos_dir, f)
            vid_dt = get_video_timestamp(full_path)
            if vid_dt:
                videos.append((vid_dt, f))
    
    videos.sort()  # Trie par date

    with open(logs_file, 'r', encoding='utf-8') as lf, open(output_file, 'w', encoding='utf-8') as of:
        # HTML avec CSS simple et JS pour langues
        of.write("<!DOCTYPE html>\n<html lang='fr'>\n<head>\n")
        of.write("<meta charset='UTF-8'>\n<title>Logs avec Vidéos</title>\n")
        of.write("<style>\n")
        of.write("body { font-family: Arial, sans-serif; margin: 0 20px 20px 20px; }\n")  # Marge top à 0
        of.write("table { border-collapse: collapse; width: 100%; }\n")
        of.write("th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n")
        of.write("th { background-color: #f2f2f2; }\n")
        of.write("a { color: #007bff; text-decoration: none; }\n")
        of.write("a:hover { text-decoration: underline; }\n")
        of.write("img { max-width: 150px; max-height: 100px; cursor: pointer; }\n")
        of.write("button { margin: 5px; width: 40px; height: 25px; border: 1px solid #000; font-size: 10px; color: white; }\n")
        of.write("#fr { background: linear-gradient(to right, #0055A4 33%, #FFFFFF 33%, #FFFFFF 66%, #EF4135 66%); }\n")  # Bleu Blanc Rouge (France)
        of.write("#lv { background: linear-gradient(to bottom, #9E3039 33%, #FFFFFF 33%, #FFFFFF 66%, #9E3039 66%); }\n")  # Bordeaux Blanc Bordeaux (Lettonie)
        of.write(".header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px; }\n")
        of.write(".header svg { width: 300px; height: 70px; }\n")  # Taille ajustée pour le nouveau SVG
        of.write("</style>\n</head>\n<body>\n")
        # En-tête avec SVG et boutons
        of.write("<div class='header'>\n")
        of.write(SVG_CONTENT)
        of.write("<div>\n")
        of.write("<button id='fr'>FR</button>\n")
        of.write("<button id='lv'>LV</button>\n")
        of.write("</div>\n")
        of.write("</div>\n")
        # Titre
        of.write("<h1 data-fr='Correspondances Logs - Vidéos' data-lv='Žurnālu un video atbilstības'>Correspondances Logs - Vidéos</h1>\n")
        of.write("<table>\n")
        of.write("<tr><th data-fr='Alarme' data-lv='Trauksme'>Alarme</th><th data-fr='Heure caméra' data-lv='Kameras laiks'>Heure caméra</th><th data-fr='Vidéo' data-lv='Video'>Vidéo</th><th data-fr='Image' data-lv='Attēls'>Image</th><th data-fr='Vitesse (km/h)' data-lv='Ātrums (km/h)'>Vitesse (km/h)</th><th data-fr='Décalage (s)' data-lv='Novirze (s)'>Décalage (s)</th><th data-fr='Détails' data-lv='Detaļas'>Détails</th></tr>\n")
        
        for line in lf:
            if not line.strip():
                continue
            camera_dt, name, speed, full_line = parse_log_line(line)
            if camera_dt is None:
                of.write(f"<tr><td data-fr='Erreur parsing' data-lv='Parsēšanas kļūda'>Erreur parsing</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td><td>{line.strip()}</td></tr>\n")
                continue
            video = find_closest_video(camera_dt, videos)
            delta_t = DISTANCE_METRES / speed
            camera_time_str = camera_dt.strftime('%Y-%m-%d %H:%M:%S')
            speed_kmh = round(speed * 3.6)  # Arrondi à l'entier le plus proche
            if video:
                # Lien absolu avec file:///
                video_path = os.path.join(videos_dir, video).replace('\\', '/')
                video_link = f"file:///{video_path}"
                # Extrait le frame avec liste des capteurs
                vid_start_dt = next((dt for dt, f in videos if f == video), None)
                if vid_start_dt:
                    offset_sec = (camera_dt - vid_start_dt).total_seconds()
                    # Nom unique avec heure de l'alarme
                    thumb_filename = f"{os.path.splitext(video)[0]}_{camera_dt.strftime('%H%M%S')}_frame.jpg"
                    thumb_path = os.path.join(thumbnails_dir, thumb_filename)
                    extract_video_frame(os.path.join(videos_dir, video), offset_sec, thumb_path, name)
                    if os.path.isfile(thumb_path):
                        thumb_link = f"./{thumbnails_dir}/{thumb_filename}"
                        thumb_html = f"<a href='{thumb_link}' target='_blank'><img src='{thumb_link}' alt='Frame avec capteurs' data-fr='Frame avec capteurs' data-lv='Kadrs ar sensoriem'></a>"
                    else:
                        thumb_html = f"<span data-fr='Erreur extraction' data-lv='Ekstrakcijas kļūda'>Erreur extraction</span>"
                else:
                    thumb_html = "-"
                of.write(f"<tr><td>{name}</td><td>{camera_time_str}</td><td><a href='{video_link}'>{video}</a></td><td>{thumb_html}</td><td>{speed_kmh}</td><td>{delta_t:.2f}</td><td>{full_line}</td></tr>\n")
            else:
                of.write(f"<tr><td>{name}</td><td>{camera_time_str}</td><td data-fr='Aucune vidéo trouvée' data-lv='Nav atrasta neviena video'>Aucune vidéo trouvée</td><td>-</td><td>{speed_kmh}</td><td>{delta_t:.2f}</td><td>{full_line}</td></tr>\n")
        
        of.write("</table>\n")
        # JS pour langues
        of.write("<script>\n")
        of.write("document.getElementById('fr').addEventListener('click', () => setLang('fr'));\n")
        of.write("document.getElementById('lv').addEventListener('click', () => setLang('lv'));\n")
        of.write("function setLang(lang) {\n")
        of.write("  document.querySelectorAll('[data-' + lang + ']').forEach(el => {\n")
        of.write("    el.textContent = el.getAttribute('data-' + lang);\n")
        of.write("  });\n")
        of.write("  document.documentElement.lang = lang;\n")
        of.write("}\n")
        of.write("</script>\n")
        of.write("</body>\n</html>\n")
    
    print(f"Traitement terminé ! Résultat dans '{output_file}' (ouvre-le dans ton navigateur pour les liens cliquables et images)")
    # Aperçu (texte brut)
    try:
        with open(output_file, 'r', encoding='utf-8') as f:
            content = f.read()
            print("Aperçu du HTML (premières lignes) :")
            print(content.split('\n')[:10])
    except:
        pass

if __name__ == '__main__':
    main()