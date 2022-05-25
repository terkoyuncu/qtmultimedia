/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QAMBISONICDECODERDATA_P_H
#define QAMBISONICDECODERDATA_P_H

#include <qtmultimediaglobal_p.h>

//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

// This file is generated by the matlab/octave file adt_generate_qt.m
// using the Ambisonic Decoder Toolbox (https://bitbucket.org/ambidecodertoolbox/adt/src/master/)


QT_BEGIN_NAMESPACE

// Decoder matrix for 5dot0, ambisonic level 1
static constexpr float decoderMatrix_5dot0_1_lf[5*4] = {
0.170387f, 0.287251f, 0.000000f, 0.257639f, // L
0.170382f, -0.287251f, 0.000000f, 0.257633f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, // C
0.368113f, 0.415955f, 0.000000f, -0.419052f, // Ls
0.368117f, -0.415960f, 0.000000f, -0.419047f, // Rs
};

// Decoder matrix for 5dot0, ambisonic level 1
static constexpr float decoderMatrix_5dot0_1_hf[5*4] = {
0.240964f, 0.234540f, 0.000000f, 0.210361f, // L
0.240957f, -0.234540f, 0.000000f, 0.210357f, // R
0.127854f, 0.000000f, 0.000000f, 0.179246f, // C
0.520591f, 0.339626f, 0.000000f, -0.342155f, // Ls
0.520596f, -0.339630f, 0.000000f, -0.342151f, // Rs
};

// Decoder matrix for 5dot0, ambisonic level 2
static constexpr float decoderMatrix_5dot0_2_lf[5*9] = {
0.170387f, 0.287251f, 0.000000f, 0.257639f, 0.105006f, 0.000000f, -0.063536f, 0.000000f, -0.008672f, // L
0.170382f, -0.287251f, 0.000000f, 0.257633f, -0.105005f, 0.000000f, -0.063562f, 0.000000f, -0.008673f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, 0.000000f, 0.000000f, -0.037403f, 0.000000f, 0.074007f, // C
0.368113f, 0.415955f, 0.000000f, -0.419052f, -0.027986f, 0.000000f, -0.092715f, 0.000000f, -0.020532f, // Ls
0.368117f, -0.415960f, 0.000000f, -0.419047f, 0.027986f, 0.000000f, -0.092693f, 0.000000f, -0.020530f, // Rs
};

// Decoder matrix for 5dot0, ambisonic level 2
static constexpr float decoderMatrix_5dot0_2_hf[5*9] = {
0.269405f, 0.351810f, 0.000000f, 0.315542f, 0.066412f, 0.000000f, -0.040184f, 0.000000f, -0.005484f, // L
0.269398f, -0.351809f, 0.000000f, 0.315535f, -0.066411f, 0.000000f, -0.040200f, 0.000000f, -0.005485f, // R
0.142945f, 0.000000f, 0.000000f, 0.268870f, 0.000000f, 0.000000f, -0.023656f, 0.000000f, 0.046806f, // C
0.582038f, 0.509439f, 0.000000f, -0.513232f, -0.017700f, 0.000000f, -0.058638f, 0.000000f, -0.012985f, // Ls
0.582043f, -0.509444f, 0.000000f, -0.513226f, 0.017700f, 0.000000f, -0.058624f, 0.000000f, -0.012985f, // Rs
};

// Decoder matrix for 5dot0, ambisonic level 3
static constexpr float decoderMatrix_5dot0_3_lf[5*16] = {
0.170387f, 0.287251f, 0.000000f, 0.257639f, 0.105006f, 0.000000f, -0.063536f, 0.000000f, -0.008672f, 0.008948f, 0.000000f, 0.020158f, 0.000000f, 0.017107f, 0.000000f, -0.009476f, // L
0.170382f, -0.287251f, 0.000000f, 0.257633f, -0.105005f, 0.000000f, -0.063562f, 0.000000f, -0.008673f, -0.008948f, 0.000000f, -0.020151f, 0.000000f, 0.017099f, 0.000000f, -0.009476f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, 0.000000f, 0.000000f, -0.037403f, 0.000000f, 0.074007f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.012666f, 0.000000f, 0.013652f, // C
0.368113f, 0.415955f, 0.000000f, -0.419052f, -0.027986f, 0.000000f, -0.092715f, 0.000000f, -0.020532f, -0.008428f, 0.000000f, 0.033196f, 0.000000f, -0.049151f, 0.000000f, -0.001179f, // Ls
0.368117f, -0.415960f, 0.000000f, -0.419047f, 0.027986f, 0.000000f, -0.092693f, 0.000000f, -0.020530f, 0.008428f, 0.000000f, -0.033195f, 0.000000f, -0.049148f, 0.000000f, -0.001179f, // Rs
};

// Decoder matrix for 5dot0, ambisonic level 3
static constexpr float decoderMatrix_5dot0_3_hf[5*16] = {
0.284237f, 0.412646f, 0.000000f, 0.370107f, 0.107262f, 0.000000f, -0.064901f, 0.000000f, -0.008858f, 0.004549f, 0.000000f, 0.010248f, 0.000000f, 0.008697f, 0.000000f, -0.004818f, // L
0.284229f, -0.412646f, 0.000000f, 0.370099f, -0.107261f, 0.000000f, -0.064928f, 0.000000f, -0.008859f, -0.004549f, 0.000000f, -0.010244f, 0.000000f, 0.008693f, 0.000000f, -0.004817f, // R
0.150814f, 0.000000f, 0.000000f, 0.315364f, 0.000000f, 0.000000f, -0.038207f, 0.000000f, 0.075597f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.006439f, 0.000000f, 0.006940f, // C
0.614081f, 0.597533f, 0.000000f, -0.601983f, -0.028587f, 0.000000f, -0.094707f, 0.000000f, -0.020973f, -0.004284f, 0.000000f, 0.016876f, 0.000000f, -0.024987f, 0.000000f, -0.000599f, // Ls
0.614086f, -0.597540f, 0.000000f, -0.601975f, 0.028587f, 0.000000f, -0.094684f, 0.000000f, -0.020972f, 0.004284f, 0.000000f, -0.016876f, 0.000000f, -0.024985f, 0.000000f, -0.000599f, // Rs
};

// Decoder matrix for 5dot1, ambisonic level 1
static constexpr float decoderMatrix_5dot1_1_lf[6*4] = {
0.170387f, 0.287251f, 0.000000f, 0.257639f, // L
0.170382f, -0.287251f, 0.000000f, 0.257633f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, // C
0.5f, 0.0f, 0.0f, 0.0f, // LFE
0.368113f, 0.415955f, 0.000000f, -0.419052f, // Ls
0.368117f, -0.415960f, 0.000000f, -0.419047f, // Rs
};

// Decoder matrix for 5dot1, ambisonic level 1
static constexpr float decoderMatrix_5dot1_1_hf[6*4] = {
0.240964f, 0.234540f, 0.000000f, 0.210361f, // L
0.240957f, -0.234540f, 0.000000f, 0.210357f, // R
0.127854f, 0.000000f, 0.000000f, 0.179246f, // C
0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.520591f, 0.339626f, 0.000000f, -0.342155f, // Ls
0.520596f, -0.339630f, 0.000000f, -0.342151f, // Rs
};

// Decoder matrix for 5dot1, ambisonic level 2
static constexpr float decoderMatrix_5dot1_2_lf[6*9] = {
0.170387f, 0.287251f, 0.000000f, 0.257639f, 0.105006f, 0.000000f, -0.063536f, 0.000000f, -0.008672f, // L
0.170382f, -0.287251f, 0.000000f, 0.257633f, -0.105005f, 0.000000f, -0.063562f, 0.000000f, -0.008673f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, 0.000000f, 0.000000f, -0.037403f, 0.000000f, 0.074007f, // C
0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.368113f, 0.415955f, 0.000000f, -0.419052f, -0.027986f, 0.000000f, -0.092715f, 0.000000f, -0.020532f, // Ls
0.368117f, -0.415960f, 0.000000f, -0.419047f, 0.027986f, 0.000000f, -0.092693f, 0.000000f, -0.020530f, // Rs
};

// Decoder matrix for 5dot1, ambisonic level 2
static constexpr float decoderMatrix_5dot1_2_hf[6*9] = {
0.269405f, 0.351810f, 0.000000f, 0.315542f, 0.066412f, 0.000000f, -0.040184f, 0.000000f, -0.005484f, // L
0.269398f, -0.351809f, 0.000000f, 0.315535f, -0.066411f, 0.000000f, -0.040200f, 0.000000f, -0.005485f, // R
0.142945f, 0.000000f, 0.000000f, 0.268870f, 0.000000f, 0.000000f, -0.023656f, 0.000000f, 0.046806f, // C
0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.582038f, 0.509439f, 0.000000f, -0.513232f, -0.017700f, 0.000000f, -0.058638f, 0.000000f, -0.012985f, // Ls
0.582043f, -0.509444f, 0.000000f, -0.513226f, 0.017700f, 0.000000f, -0.058624f, 0.000000f, -0.012985f, // Rs
};

// Decoder matrix for 5dot1, ambisonic level 3
static constexpr float decoderMatrix_5dot1_3_lf[6*16] = {
0.170387f, 0.287251f, 0.000000f, 0.257639f, 0.105006f, 0.000000f, -0.063536f, 0.000000f, -0.008672f, 0.008948f, 0.000000f, 0.020158f, 0.000000f, 0.017107f, 0.000000f, -0.009476f, // L
0.170382f, -0.287251f, 0.000000f, 0.257633f, -0.105005f, 0.000000f, -0.063562f, 0.000000f, -0.008673f, -0.008948f, 0.000000f, -0.020151f, 0.000000f, 0.017099f, 0.000000f, -0.009476f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, 0.000000f, 0.000000f, -0.037403f, 0.000000f, 0.074007f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.012666f, 0.000000f, 0.013652f, // C
0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.368113f, 0.415955f, 0.000000f, -0.419052f, -0.027986f, 0.000000f, -0.092715f, 0.000000f, -0.020532f, -0.008428f, 0.000000f, 0.033196f, 0.000000f, -0.049151f, 0.000000f, -0.001179f, // Ls
0.368117f, -0.415960f, 0.000000f, -0.419047f, 0.027986f, 0.000000f, -0.092693f, 0.000000f, -0.020530f, 0.008428f, 0.000000f, -0.033195f, 0.000000f, -0.049148f, 0.000000f, -0.001179f, // Rs
};

// Decoder matrix for 5dot1, ambisonic level 3
static constexpr float decoderMatrix_5dot1_3_hf[6*16] = {
0.284237f, 0.412646f, 0.000000f, 0.370107f, 0.107262f, 0.000000f, -0.064901f, 0.000000f, -0.008858f, 0.004549f, 0.000000f, 0.010248f, 0.000000f, 0.008697f, 0.000000f, -0.004818f, // L
0.284229f, -0.412646f, 0.000000f, 0.370099f, -0.107261f, 0.000000f, -0.064928f, 0.000000f, -0.008859f, -0.004549f, 0.000000f, -0.010244f, 0.000000f, 0.008693f, 0.000000f, -0.004817f, // R
0.150814f, 0.000000f, 0.000000f, 0.315364f, 0.000000f, 0.000000f, -0.038207f, 0.000000f, 0.075597f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.006439f, 0.000000f, 0.006940f, // C
0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.614081f, 0.597533f, 0.000000f, -0.601983f, -0.028587f, 0.000000f, -0.094707f, 0.000000f, -0.020973f, -0.004284f, 0.000000f, 0.016876f, 0.000000f, -0.024987f, 0.000000f, -0.000599f, // Ls
0.614086f, -0.597540f, 0.000000f, -0.601975f, 0.028587f, 0.000000f, -0.094684f, 0.000000f, -0.020972f, 0.004284f, 0.000000f, -0.016876f, 0.000000f, -0.024985f, 0.000000f, -0.000599f, // Rs
};

// Decoder matrix for 7dot0, ambisonic level 1
static constexpr float decoderMatrix_7dot0_1_lf[7*4] = {
0.137267f, 0.209910f, 0.000000f, 0.244088f, // L
0.137255f, -0.209914f, 0.000000f, 0.244084f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, // C
0.184133f, 0.413172f, 0.000000f, 0.000000f, // Ls
0.184152f, -0.413179f, 0.000000f, 0.000000f, // Rs
0.184148f, 0.206589f, 0.000000f, -0.357822f, // Lb
0.184131f, -0.206595f, 0.000000f, -0.357815f, // Rb
};

// Decoder matrix for 7dot0, ambisonic level 1
static constexpr float decoderMatrix_7dot0_1_hf[7*4] = {
0.194124f, 0.171391f, 0.000000f, 0.199297f, // L
0.194108f, -0.171394f, 0.000000f, 0.199294f, // R
0.127854f, 0.000000f, 0.000000f, 0.179246f, // C
0.260404f, 0.337353f, 0.000000f, 0.000000f, // Ls
0.260430f, -0.337359f, 0.000000f, 0.000000f, // Rs
0.260425f, 0.168680f, 0.000000f, -0.292160f, // Lb
0.260400f, -0.168684f, 0.000000f, -0.292155f, // Rb
};

// Decoder matrix for 7dot0, ambisonic level 2
static constexpr float decoderMatrix_7dot0_2_lf[7*9] = {
0.137267f, 0.209910f, 0.000000f, 0.244088f, 0.096579f, 0.000000f, -0.054359f, 0.000000f, 0.015569f, // L
0.137255f, -0.209914f, 0.000000f, 0.244084f, -0.096578f, 0.000000f, -0.054434f, 0.000000f, 0.015565f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, 0.000000f, 0.000000f, -0.037403f, 0.000000f, 0.074007f, // C
0.184133f, 0.413172f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.071326f, 0.000000f, -0.108845f, // Ls
0.184152f, -0.413179f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.071237f, 0.000000f, -0.108845f, // Rs
0.184148f, 0.206589f, 0.000000f, -0.357822f, -0.094264f, 0.000000f, -0.071249f, 0.000000f, 0.054422f, // Lb
0.184131f, -0.206595f, 0.000000f, -0.357815f, 0.094263f, 0.000000f, -0.071359f, 0.000000f, 0.054419f, // Rb
};

// Decoder matrix for 7dot0, ambisonic level 2
static constexpr float decoderMatrix_7dot0_2_hf[7*9] = {
0.217038f, 0.257087f, 0.000000f, 0.298946f, 0.061082f, 0.000000f, -0.034380f, 0.000000f, 0.009847f, // L
0.217019f, -0.257091f, 0.000000f, 0.298940f, -0.061081f, 0.000000f, -0.034427f, 0.000000f, 0.009844f, // R
0.142945f, 0.000000f, 0.000000f, 0.268870f, 0.000000f, 0.000000f, -0.023656f, 0.000000f, 0.046806f, // C
0.291140f, 0.506030f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.045111f, 0.000000f, -0.068839f, // Ls
0.291170f, -0.506039f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.045054f, 0.000000f, -0.068839f, // Rs
0.291164f, 0.253019f, 0.000000f, -0.438240f, -0.059618f, 0.000000f, -0.045062f, 0.000000f, 0.034420f, // Lb
0.291136f, -0.253026f, 0.000000f, -0.438232f, 0.059617f, 0.000000f, -0.045131f, 0.000000f, 0.034417f, // Rb
};

// Decoder matrix for 7dot0, ambisonic level 3
static constexpr float decoderMatrix_7dot0_3_lf[7*16] = {
0.137267f, 0.209910f, 0.000000f, 0.244088f, 0.096579f, 0.000000f, -0.054359f, 0.000000f, 0.015569f, 0.012993f, 0.000000f, 0.013238f, 0.000000f, 0.015122f, 0.000000f, -0.007377f, // L
0.137255f, -0.209914f, 0.000000f, 0.244084f, -0.096578f, 0.000000f, -0.054434f, 0.000000f, 0.015565f, -0.012992f, 0.000000f, -0.013220f, 0.000000f, 0.015113f, 0.000000f, -0.007377f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, 0.000000f, 0.000000f, -0.037403f, 0.000000f, 0.074007f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.012666f, 0.000000f, 0.013652f, // C
0.184133f, 0.413172f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.071326f, 0.000000f, -0.108845f, -0.012334f, 0.000000f, 0.026779f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, // Ls
0.184152f, -0.413179f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.071237f, 0.000000f, -0.108845f, 0.012334f, 0.000000f, -0.026796f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, // Rs
0.184148f, 0.206589f, 0.000000f, -0.357822f, -0.094264f, 0.000000f, -0.071249f, 0.000000f, 0.054422f, 0.012334f, 0.000000f, 0.013396f, 0.000000f, -0.023206f, 0.000000f, 0.000000f, // Lb
0.184131f, -0.206595f, 0.000000f, -0.357815f, 0.094263f, 0.000000f, -0.071359f, 0.000000f, 0.054419f, -0.012333f, 0.000000f, -0.013386f, 0.000000f, -0.023186f, 0.000000f, 0.000000f, // Rb
};

// Decoder matrix for 7dot0, ambisonic level 3
static constexpr float decoderMatrix_7dot0_3_hf[7*16] = {
0.228986f, 0.301543f, 0.000000f, 0.350641f, 0.098654f, 0.000000f, -0.055527f, 0.000000f, 0.015903f, 0.006605f, 0.000000f, 0.006730f, 0.000000f, 0.007688f, 0.000000f, -0.003750f, // L
0.228967f, -0.301549f, 0.000000f, 0.350635f, -0.098653f, 0.000000f, -0.055604f, 0.000000f, 0.015899f, -0.006605f, 0.000000f, -0.006721f, 0.000000f, 0.007683f, 0.000000f, -0.003750f, // R
0.150814f, 0.000000f, 0.000000f, 0.315364f, 0.000000f, 0.000000f, -0.038207f, 0.000000f, 0.075597f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.006439f, 0.000000f, 0.006940f, // C
0.307168f, 0.593535f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.072859f, 0.000000f, -0.111183f, -0.006270f, 0.000000f, 0.013613f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, // Ls
0.307199f, -0.593546f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.072768f, 0.000000f, -0.111183f, 0.006270f, 0.000000f, -0.013622f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, // Rs
0.307193f, 0.296773f, 0.000000f, -0.514023f, -0.096289f, 0.000000f, -0.072780f, 0.000000f, 0.055592f, 0.006270f, 0.000000f, 0.006810f, 0.000000f, -0.011797f, 0.000000f, 0.000000f, // Lb
0.307164f, -0.296781f, 0.000000f, -0.514014f, 0.096288f, 0.000000f, -0.072892f, 0.000000f, 0.055588f, -0.006270f, 0.000000f, -0.006805f, 0.000000f, -0.011787f, 0.000000f, 0.000000f, // Rb
};

// Decoder matrix for 7dot1, ambisonic level 1
static constexpr float decoderMatrix_7dot1_1_lf[8*4] = {
0.137267f, 0.209910f, 0.000000f, 0.244088f, // L
0.137255f, -0.209914f, 0.000000f, 0.244084f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, // C
0.5f, 0.0f, 0.0f, 0.0f, // LFE
0.184133f, 0.413172f, 0.000000f, 0.000000f, // Ls
0.184152f, -0.413179f, 0.000000f, 0.000000f, // Rs
0.184148f, 0.206589f, 0.000000f, -0.357822f, // Lb
0.184131f, -0.206595f, 0.000000f, -0.357815f, // Rb
};

// Decoder matrix for 7dot1, ambisonic level 1
static constexpr float decoderMatrix_7dot1_1_hf[8*4] = {
0.194124f, 0.171391f, 0.000000f, 0.199297f, // L
0.194108f, -0.171394f, 0.000000f, 0.199294f, // R
0.127854f, 0.000000f, 0.000000f, 0.179246f, // C
0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.260404f, 0.337353f, 0.000000f, 0.000000f, // Ls
0.260430f, -0.337359f, 0.000000f, 0.000000f, // Rs
0.260425f, 0.168680f, 0.000000f, -0.292160f, // Lb
0.260400f, -0.168684f, 0.000000f, -0.292155f, // Rb
};

// Decoder matrix for 7dot1, ambisonic level 2
static constexpr float decoderMatrix_7dot1_2_lf[8*9] = {
0.137267f, 0.209910f, 0.000000f, 0.244088f, 0.096579f, 0.000000f, -0.054359f, 0.000000f, 0.015569f, // L
0.137255f, -0.209914f, 0.000000f, 0.244084f, -0.096578f, 0.000000f, -0.054434f, 0.000000f, 0.015565f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, 0.000000f, 0.000000f, -0.037403f, 0.000000f, 0.074007f, // C
0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.184133f, 0.413172f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.071326f, 0.000000f, -0.108845f, // Ls
0.184152f, -0.413179f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.071237f, 0.000000f, -0.108845f, // Rs
0.184148f, 0.206589f, 0.000000f, -0.357822f, -0.094264f, 0.000000f, -0.071249f, 0.000000f, 0.054422f, // Lb
0.184131f, -0.206595f, 0.000000f, -0.357815f, 0.094263f, 0.000000f, -0.071359f, 0.000000f, 0.054419f, // Rb
};

// Decoder matrix for 7dot1, ambisonic level 2
static constexpr float decoderMatrix_7dot1_2_hf[8*9] = {
0.217038f, 0.257087f, 0.000000f, 0.298946f, 0.061082f, 0.000000f, -0.034380f, 0.000000f, 0.009847f, // L
0.217019f, -0.257091f, 0.000000f, 0.298940f, -0.061081f, 0.000000f, -0.034427f, 0.000000f, 0.009844f, // R
0.142945f, 0.000000f, 0.000000f, 0.268870f, 0.000000f, 0.000000f, -0.023656f, 0.000000f, 0.046806f, // C
0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.291140f, 0.506030f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.045111f, 0.000000f, -0.068839f, // Ls
0.291170f, -0.506039f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.045054f, 0.000000f, -0.068839f, // Rs
0.291164f, 0.253019f, 0.000000f, -0.438240f, -0.059618f, 0.000000f, -0.045062f, 0.000000f, 0.034420f, // Lb
0.291136f, -0.253026f, 0.000000f, -0.438232f, 0.059617f, 0.000000f, -0.045131f, 0.000000f, 0.034417f, // Rb
};

// Decoder matrix for 7dot1, ambisonic level 3
static constexpr float decoderMatrix_7dot1_3_lf[8*16] = {
0.137267f, 0.209910f, 0.000000f, 0.244088f, 0.096579f, 0.000000f, -0.054359f, 0.000000f, 0.015569f, 0.012993f, 0.000000f, 0.013238f, 0.000000f, 0.015122f, 0.000000f, -0.007377f, // L
0.137255f, -0.209914f, 0.000000f, 0.244084f, -0.096578f, 0.000000f, -0.054434f, 0.000000f, 0.015565f, -0.012992f, 0.000000f, -0.013220f, 0.000000f, 0.015113f, 0.000000f, -0.007377f, // R
0.090406f, 0.000000f, 0.000000f, 0.219531f, 0.000000f, 0.000000f, -0.037403f, 0.000000f, 0.074007f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.012666f, 0.000000f, 0.013652f, // C
0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.184133f, 0.413172f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.071326f, 0.000000f, -0.108845f, -0.012334f, 0.000000f, 0.026779f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, // Ls
0.184152f, -0.413179f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.071237f, 0.000000f, -0.108845f, 0.012334f, 0.000000f, -0.026796f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, // Rs
0.184148f, 0.206589f, 0.000000f, -0.357822f, -0.094264f, 0.000000f, -0.071249f, 0.000000f, 0.054422f, 0.012334f, 0.000000f, 0.013396f, 0.000000f, -0.023206f, 0.000000f, 0.000000f, // Lb
0.184131f, -0.206595f, 0.000000f, -0.357815f, 0.094263f, 0.000000f, -0.071359f, 0.000000f, 0.054419f, -0.012333f, 0.000000f, -0.013386f, 0.000000f, -0.023186f, 0.000000f, 0.000000f, // Rb
};

// Decoder matrix for 7dot1, ambisonic level 3
static constexpr float decoderMatrix_7dot1_3_hf[8*16] = {
0.228986f, 0.301543f, 0.000000f, 0.350641f, 0.098654f, 0.000000f, -0.055527f, 0.000000f, 0.015903f, 0.006605f, 0.000000f, 0.006730f, 0.000000f, 0.007688f, 0.000000f, -0.003750f, // L
0.228967f, -0.301549f, 0.000000f, 0.350635f, -0.098653f, 0.000000f, -0.055604f, 0.000000f, 0.015899f, -0.006605f, 0.000000f, -0.006721f, 0.000000f, 0.007683f, 0.000000f, -0.003750f, // R
0.150814f, 0.000000f, 0.000000f, 0.315364f, 0.000000f, 0.000000f, -0.038207f, 0.000000f, 0.075597f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.006439f, 0.000000f, 0.006940f, // C
0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // LFE
0.307168f, 0.593535f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.072859f, 0.000000f, -0.111183f, -0.006270f, 0.000000f, 0.013613f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, // Ls
0.307199f, -0.593546f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.072768f, 0.000000f, -0.111183f, 0.006270f, 0.000000f, -0.013622f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, // Rs
0.307193f, 0.296773f, 0.000000f, -0.514023f, -0.096289f, 0.000000f, -0.072780f, 0.000000f, 0.055592f, 0.006270f, 0.000000f, 0.006810f, 0.000000f, -0.011797f, 0.000000f, 0.000000f, // Lb
0.307164f, -0.296781f, 0.000000f, -0.514014f, 0.096288f, 0.000000f, -0.072892f, 0.000000f, 0.055588f, -0.006270f, 0.000000f, -0.006805f, 0.000000f, -0.011787f, 0.000000f, 0.000000f, // Rb
};

QT_END_NAMESPACE

#endif

