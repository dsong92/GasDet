#!/usr/bin/env python
# -*- coding: utf-8 -*-
import csv
import numpy as np
from scipy.interpolate import interp2d
from scipy.interpolate import interp1d
from matplotlib import pyplot as plt
import pandas as pd
import os
import re
import subprocess

beam_shift = [-150, -100, -50, 0, 50, 100, 150]
beam_height = [-25, -26, -27]
electrode_width = [15, 20, 25]
electrode_length =[80, 100, 120]
electric_field = [500, 1000, 1500]

run_script_manual = open("d2RunScript.txt","w")
line_jumper = 0

for i, shift in enumerate(beam_shift):
    for j, height in enumerate(beam_height):
        for k, width in enumerate(electrode_width):
            for l, length in enumerate(electrode_length):
                for m, field in enumerate(electric_field):
                    tmp_macro = open("{}um_{}mmUp_W{}mm_L{}mm_{}Vpercm.mac".format(shift, 30+height, width*2, length*2, field), "w")
                    tmp_macro.write("/control/verbose 0\n")
                    tmp_macro.write("/run/verbose 0\n")
                    tmp_macro.write("/analysis/setFileName {}um_{}mmUp_W{}mm_L{}mm_{}Vpercm\n".format(shift, 30+height, width*2, length*2, field))
                    tmp_macro.write("/beam/pos/shift {}\n".format(shift))
                    tmp_macro.write("/beam/pos/up {}\n".format(height))
                    tmp_macro.write("/beam/pdist 162harmonic.d2\n")
                    tmp_macro.write("/det/electrode/width {}\n".format(width))
                    tmp_macro.write("/det/electrode/length {}\n".format(length))
                    tmp_macro.write("/det/Efield {}".format(field))
                    tmp_macro.close()

                    if line_jumper != 0 and line_jumper % 70 == 0:
                        run_script_manual.write("exampleB1.exe {}um_{}mmUp_W{}mm_L{}mm_{}Vpercm.mac \n".format(shift, 30+height, width*2, length*2, field))
                    else:
                        run_script_manual.write("exampleB1.exe {}um_{}mmUp_W{}mm_L{}mm_{}Vpercm.mac && ".format(shift, 30+height, width*2, length*2, field))
                    line_jumper += 1

run_script_manual.close()


'''
hadd_script_manual = open("hadd.txt","a")
hadd_script_manual.write("hadd all.root ")
for i, name in enumerate(result_file_name_sorted_by_harmonicN):
    #print(name)
    hadd_script_manual.write("{}.root ".format(macro_file_name_sorted_by_harmonicN[i]))
hadd_script_manual.close()

# photon portion 정할 excel 읽고 값 저장하기
photon_flux_raw_data = pd.read_excel("./../10m_20keV_1eV_5x3mm.xlsx")


#---- 매크로파일 만드는 부분 시작
run_script_manual = open("RunScript.txt","a")
line_jumper = 0
for mac_num, FD_harmonic in enumerate(macro_file_name_sorted_by_harmonicN):
    #macro_file = open("myscrip.mac","w")
    macro_file = open("{}.mac".format(macro_file_name_sorted_by_harmonicN[mac_num]), "w")

    #df = pd.read_excel("1_30eV.xlsx")
    df = pd.read_excel("{}.xlsx".format(FD_harmonic))

    FD_raw_data_XY = pd.DataFrame.to_numpy(df)
    # X, Y, Height(or Weight) : array
    FD_raw_data_X = []
    FD_raw_data_Y = []
    FD_raw_data_H = []
    FD_raw_data_tmp_var = []
    # weight의 2-dim array를 저장할 변수
    FD_raw_data_Bincontent = []


    # 2d interp.
    FD_interp2d = interp2d(interp2d_Xrange, interp2d_Yrange, FD_raw_data_Bincontent, kind='cubic')
    FD_interp2d_H = FD_interp2d(interp2d_X, interp2d_Y)
    FD_interp2d_H = np.array(FD_interp2d_H)
    #print(FD_interp2d_H.shape)
    #print(FD_interp2d_H.flatten().shape)

    # 모든 지점의 weight 값의 합 구해놓음
    scale_down_variable = 10000000000 # 1.0E10
    flux_sum = 0
    tmp_loop1 = 0
    for yi in range(30):
        for xi in range(50):
            #flux_sum += float(FD_interp2d_H.flatten()[tmp_loop1]/scale_down_variable)
            flux_sum += float(FD_interp2d_H.flatten()[tmp_loop1])
            tmp_loop1 += 1
    flux_sum_int = int(flux_sum/scale_down_variable)
    #print(flux_sum)
    #print(flux_sum_int)
    #print(int(flux_sum))

    # 매크로 들어갈 내용 시작
    macro_file.write("/tracking/verbose 0\n")
    macro_file.write("/control/verbose 0\n")
    macro_file.write("/run/verbose 2\n")
    macro_file.write("/run/initialize\n")
    macro_file.write("/process/list\n")

    # 2d interp. 는 2 dim array (n,m) --> flatten() --> (n*m,) 각 요소 끄집어 내기위한 변수
    loop_for_Weight = 0
    # y 값 -1.5 ~ 1.5를 30으로 나눔
    delta = 0.1
    # 한 라인 스캔 끝날 때 마다 delta * number --> 이 number
    slicing_y = 0
    for yi in range(30):
        for xi in range(50):
            #print("X-->",Xi[xi],"|| Y-->", Yi[50*yi+yi],"|| H --> ",FD_interp2d_H.flatten()[xi*yi+xi])
            if yi == 0 and xi == 0:
                #print("/gps/source/intensity ",int(FD_interp2d_H.flatten()[xi*yi+xi]/flux_sum))
                macro_file.write("/gps/source/intensity {}\n".format(float(FD_interp2d_H.flatten()[loop_for_Weight]/flux_sum)))
                macro_file.write("/gps/pos/type Plane\n")
                macro_file.write("/gps/pos/shape Square\n")
                macro_file.write("/gps/pos/halfx 0.05 mm\n")
                macro_file.write("/gps/pos/halfy 0.05 mm\n")
                #print("/gps/pos/centre {} {} {} mm".format(round(Xi[xi],6), round(Yi[50*yi + yi],4), -149))
                macro_file.write("/gps/pos/centre {} {} {} mm\n".format(round(Xi[xi],6) , round(-1.45 + delta*slicing_y,4), -149))
                macro_file.write("/gps/particle gamma\n")
                macro_file.write("/gps/energy {} eV\n".format(eV_list_sorted_by_harmonicN[mac_num]))
                #print("/gps/pos/type Beam")
                macro_file.write("/gps/direction 0 0.0 1.0\n")
                loop_for_Weight += 1
            else:
                #print("/gps/source/add ",int(FD_interp2d_H.flatten()[xi*yi+xi]/flux_sum))
                macro_file.write("/gps/source/add {}\n".format(float(FD_interp2d_H.flatten()[loop_for_Weight]/flux_sum)))
                macro_file.write("/gps/pos/type Plane\n")
                macro_file.write("/gps/pos/shape Square\n")
                macro_file.write("/gps/pos/halfx 0.05 mm\n")
                macro_file.write("/gps/pos/halfy 0.05 mm\n")
                #print("/gps/pos/centre {} {} {} mm".format(round(Xi[xi],6), round(Yi[50*yi + yi],4), -149))
                macro_file.write("/gps/pos/centre {} {} {} mm\n".format(round(Xi[xi], 6), round(-1.45 + delta * slicing_y,4), -149))
                macro_file.write("/gps/particle gamma\n")
                macro_file.write("/gps/energy {} eV\n".format(eV_list_sorted_by_harmonicN[mac_num]))
                #print("/gps/pos/type Beam")
                macro_file.write("/gps/direction 0 0.0 1.0\n")
                loop_for_Weight += 1
        slicing_y += 1

    macro_file.write("/analysis/setFileName {}\n".format(result_file_name_sorted_by_harmonicN[mac_num]))
    macro_file.write("/analysis/h2/setX 0 100 -5 5 mm\n")
    macro_file.write("/analysis/h2/setY 0 60 -3 3 mm\n")
    macro_file.write("/analysis/h2/setXaxis 0 X[mm]\n")
    macro_file.write("/analysis/h2/setYaxis 0 Y[mm]\n")


    macro_file.write("/run/printProgress {}\n".format(int(Number_Of_PhotonBeam/100)))
    macro_file.write("/run/beamOn {}\n".format(int(Number_Of_PhotonBeam)))

    macro_file.close()

run_script_manual.close()
'''