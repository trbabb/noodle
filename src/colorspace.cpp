#include <geomc/function/Raster.h>
#include <geomc/linalg/Matrix.h>
#include <geomc/random/RandomTools.h>
#include <geomc/shape/Sphere.h>
#include <iostream>

#include "Image.h"

using namespace geom;
using namespace std;

#define SPECTRAL_MIN_NM   390
#define SPECTRAL_MAX_NM   830
#define SPECTRAL_STEP_NM  1
#define SPECTRAL_COUNT    ((SPECTRAL_MAX_NM - SPECTRAL_MIN_NM) / SPECTRAL_STEP_NM + 1)

const float x_matching_f[] = {
    3.769647E-03,4.532416E-03,5.446553E-03,6.538868E-03,7.839699E-03,
    9.382967E-03,1.120608E-02,1.334965E-02,1.585690E-02,1.877286E-02,
    2.214302E-02,2.601285E-02,3.043036E-02,3.544325E-02,4.109640E-02,
    4.742986E-02,5.447394E-02,6.223612E-02,7.070048E-02,7.982513E-02,
    8.953803E-02,9.974848E-02,1.104019E-01,1.214566E-01,1.328741E-01,
    1.446214E-01,1.566468E-01,1.687901E-01,1.808328E-01,1.925216E-01,
    2.035729E-01,2.137531E-01,2.231348E-01,2.319245E-01,2.403892E-01,
    2.488523E-01,2.575896E-01,2.664991E-01,2.753532E-01,2.838921E-01,
    2.918246E-01,2.989200E-01,3.052993E-01,3.112031E-01,3.169047E-01,
    3.227087E-01,3.288194E-01,3.349242E-01,3.405452E-01,3.451688E-01,
    3.482554E-01,3.494153E-01,3.489075E-01,3.471746E-01,3.446705E-01,
    3.418483E-01,3.390240E-01,3.359926E-01,3.324276E-01,3.280157E-01,
    3.224637E-01,3.156225E-01,3.078201E-01,2.994771E-01,2.909776E-01,
    2.826646E-01,2.747962E-01,2.674312E-01,2.605847E-01,2.542749E-01,
    2.485254E-01,2.433039E-01,2.383414E-01,2.333253E-01,2.279619E-01,
    2.219781E-01,2.151735E-01,2.075619E-01,1.992183E-01,1.902290E-01,
    1.806905E-01,1.707154E-01,1.604471E-01,1.500244E-01,1.395705E-01,
    1.291920E-01,1.189859E-01,1.090615E-01,9.951424E-02,9.041850E-02,
    8.182895E-02,7.376817E-02,6.619477E-02,5.906380E-02,5.234242E-02,
    4.600865E-02,4.006154E-02,3.454373E-02,2.949091E-02,2.492140E-02,
    2.083981E-02,1.723591E-02,1.407924E-02,1.134516E-02,9.019658E-03,
    7.097731E-03,5.571145E-03,4.394566E-03,3.516303E-03,2.887638E-03,
    2.461588E-03,2.206348E-03,2.149559E-03,2.337091E-03,2.818931E-03,
    3.649178E-03,4.891359E-03,6.629364E-03,8.942902E-03,1.190224E-02,
    1.556989E-02,1.997668E-02,2.504698E-02,3.067530E-02,3.674999E-02,
    4.315171E-02,4.978584E-02,5.668554E-02,6.391651E-02,7.154352E-02,
    7.962917E-02,8.821473E-02,9.726978E-02,1.067504E-01,1.166192E-01,
    1.268468E-01,1.374060E-01,1.482471E-01,1.593076E-01,1.705181E-01,
    1.818026E-01,1.931090E-01,2.045085E-01,2.161166E-01,2.280650E-01,
    2.405015E-01,2.535441E-01,2.671300E-01,2.811351E-01,2.954164E-01,
    3.098117E-01,3.241678E-01,3.384319E-01,3.525786E-01,3.665839E-01,
    3.804244E-01,3.940988E-01,4.076972E-01,4.213484E-01,4.352003E-01,
    4.494206E-01,4.641616E-01,4.794395E-01,4.952180E-01,5.114395E-01,
    5.280233E-01,5.448696E-01,5.618898E-01,5.790137E-01,5.961882E-01,
    6.133784E-01,6.305897E-01,6.479223E-01,6.654866E-01,6.833782E-01,
    7.016774E-01,7.204110E-01,7.394495E-01,7.586285E-01,7.777885E-01,
    7.967750E-01,8.154530E-01,8.337389E-01,8.515493E-01,8.687862E-01,
    8.853376E-01,9.011588E-01,9.165278E-01,9.318245E-01,9.474524E-01,
    9.638388E-01,9.812596E-01,9.992953E-01,1.017343E+00,1.034790E+00,
    1.051011E+00,1.065522E+00,1.078421E+00,1.089944E+00,1.100320E+00,
    1.109767E+00,1.118438E+00,1.126266E+00,1.133138E+00,1.138952E+00,
    1.143620E+00,1.147095E+00,1.149464E+00,1.150838E+00,1.151326E+00,
    1.151033E+00,1.150002E+00,1.148061E+00,1.144998E+00,1.140622E+00,
    1.134757E+00,1.127298E+00,1.118342E+00,1.108033E+00,1.096515E+00,
    1.083928E+00,1.070387E+00,1.055934E+00,1.040592E+00,1.024385E+00,
    1.007344E+00,9.895268E-01,9.711213E-01,9.523257E-01,9.333248E-01,
    9.142877E-01,8.952798E-01,8.760157E-01,8.561607E-01,8.354235E-01,
    8.135565E-01,7.904565E-01,7.664364E-01,7.418777E-01,7.171219E-01,
    6.924717E-01,6.681600E-01,6.442697E-01,6.208450E-01,5.979243E-01,
    5.755410E-01,5.537296E-01,5.325412E-01,5.120218E-01,4.922070E-01,
    4.731224E-01,4.547417E-01,4.368719E-01,4.193121E-01,4.018980E-01,
    3.844986E-01,3.670592E-01,3.497167E-01,3.326305E-01,3.159341E-01,
    2.997374E-01,2.841189E-01,2.691053E-01,2.547077E-01,2.409319E-01,
    2.277792E-01,2.152431E-01,2.033010E-01,1.919276E-01,1.810987E-01,
    1.707914E-01,1.609842E-01,1.516577E-01,1.427936E-01,1.343737E-01,
    1.263808E-01,1.187979E-01,1.116088E-01,1.047975E-01,9.834835E-02,
    9.224597E-02,8.647506E-02,8.101986E-02,7.586514E-02,7.099633E-02,
    6.639960E-02,6.206225E-02,5.797409E-02,5.412533E-02,5.050600E-02,
    4.710606E-02,4.391411E-02,4.091411E-02,3.809067E-02,3.543034E-02,
    3.292138E-02,3.055672E-02,2.834146E-02,2.628033E-02,2.437465E-02,
    2.262306E-02,2.101935E-02,1.954647E-02,1.818727E-02,1.692727E-02,
    1.575417E-02,1.465854E-02,1.363571E-02,1.268205E-02,1.179394E-02,
    1.096778E-02,1.019964E-02,9.484317E-03,8.816851E-03,8.192921E-03,
    7.608750E-03,7.061391E-03,6.549509E-03,6.071970E-03,5.627476E-03,
    5.214608E-03,4.831848E-03,4.477579E-03,4.150166E-03,3.847988E-03,
    3.569452E-03,3.312857E-03,3.076022E-03,2.856894E-03,2.653681E-03,
    2.464821E-03,2.289060E-03,2.125694E-03,1.974121E-03,1.833723E-03,
    1.703876E-03,1.583904E-03,1.472939E-03,1.370151E-03,1.274803E-03,
    1.186238E-03,1.103871E-03,1.027194E-03,9.557493E-04,8.891262E-04,
    8.269535E-04,7.689351E-04,7.149425E-04,6.648590E-04,6.185421E-04,
    5.758303E-04,5.365046E-04,5.001842E-04,4.665005E-04,4.351386E-04,
    4.058303E-04,3.783733E-04,3.526892E-04,3.287199E-04,3.063998E-04,
    2.856577E-04,2.664108E-04,2.485462E-04,2.319529E-04,2.165300E-04,
    2.021853E-04,1.888338E-04,1.763935E-04,1.647895E-04,1.539542E-04,
    1.438270E-04,1.343572E-04,1.255141E-04,1.172706E-04,1.095983E-04,
    1.024685E-04,9.584715E-05,8.968316E-05,8.392734E-05,7.853708E-05,
    7.347551E-05,6.871576E-05,6.425257E-05,6.008292E-05,5.620098E-05,
    5.259870E-05,4.926279E-05,4.616623E-05,4.328212E-05,4.058715E-05,
    3.806114E-05,3.568818E-05,3.346023E-05,3.137090E-05,2.941371E-05,
    2.758222E-05,2.586951E-05,2.426701E-05,2.276639E-05,2.136009E-05,
    2.004122E-05,1.880380E-05,1.764358E-05,1.655671E-05,1.553939E-05,
    1.458792E-05,1.369853E-05,1.286705E-05,1.208947E-05,1.136207E-05,
    1.068141E-05,1.004411E-05,9.446399E-06,8.884754E-06,8.356050E-06,
    7.857521E-06,7.386996E-06,6.943576E-06,6.526548E-06,6.135087E-06,
    5.768284E-06,5.425069E-06,5.103974E-06,4.803525E-06,4.522350E-06,
    4.259166E-06,4.012715E-06,3.781597E-06,3.564496E-06,3.360236E-06,
    3.167765E-06,2.986206E-06,2.814999E-06,2.653663E-06,2.501725E-06,
    2.358723E-06,2.224206E-06,2.097737E-06,1.978894E-06,1.867268E-06,
    1.762465E-06
};

const float y_matching_f[] = {
    4.146161E-04,5.028333E-04,6.084991E-04,7.344436E-04,8.837389E-04,
    1.059646E-03,1.265532E-03,1.504753E-03,1.780493E-03,2.095572E-03,
    2.452194E-03,2.852216E-03,3.299115E-03,3.797466E-03,4.352768E-03,
    4.971717E-03,5.661014E-03,6.421615E-03,7.250312E-03,8.140173E-03,
    9.079860E-03,1.005608E-02,1.106456E-02,1.210522E-02,1.318014E-02,
    1.429377E-02,1.545004E-02,1.664093E-02,1.785302E-02,1.907018E-02,
    2.027369E-02,2.144805E-02,2.260041E-02,2.374789E-02,2.491247E-02,
    2.612106E-02,2.739923E-02,2.874993E-02,3.016909E-02,3.165145E-02,
    3.319038E-02,3.477912E-02,3.641495E-02,3.809569E-02,3.981843E-02,
    4.157940E-02,4.337098E-02,4.517180E-02,4.695420E-02,4.868718E-02,
    5.033657E-02,5.187611E-02,5.332218E-02,5.470603E-02,5.606335E-02,
    5.743393E-02,5.885107E-02,6.030809E-02,6.178644E-02,6.326570E-02,
    6.472352E-02,6.614749E-02,6.757256E-02,6.904928E-02,7.063280E-02,
    7.238339E-02,7.435960E-02,7.659383E-02,7.911436E-02,8.195345E-02,
    8.514816E-02,8.872657E-02,9.266008E-02,9.689723E-02,1.013746E-01,
    1.060145E-01,1.107377E-01,1.155111E-01,1.203122E-01,1.251161E-01,
    1.298957E-01,1.346299E-01,1.393309E-01,1.440235E-01,1.487372E-01,
    1.535066E-01,1.583644E-01,1.633199E-01,1.683761E-01,1.735365E-01,
    1.788048E-01,1.841819E-01,1.896559E-01,1.952101E-01,2.008259E-01,
    2.064828E-01,2.121826E-01,2.180279E-01,2.241586E-01,2.307302E-01,
    2.379160E-01,2.458706E-01,2.546023E-01,2.640760E-01,2.742490E-01,
    2.850680E-01,2.964837E-01,3.085010E-01,3.211393E-01,3.344175E-01,
    3.483536E-01,3.629601E-01,3.782275E-01,3.941359E-01,4.106582E-01,
    4.277595E-01,4.453993E-01,4.635396E-01,4.821376E-01,5.011430E-01,
    5.204972E-01,5.401387E-01,5.600208E-01,5.800972E-01,6.003172E-01,
    6.206256E-01,6.409398E-01,6.610772E-01,6.808134E-01,6.999044E-01,
    7.180890E-01,7.351593E-01,7.511821E-01,7.663143E-01,7.807352E-01,
    7.946448E-01,8.082074E-01,8.213817E-01,8.340701E-01,8.461711E-01,
    8.575799E-01,8.682408E-01,8.783061E-01,8.879907E-01,8.975211E-01,
    9.071347E-01,9.169947E-01,9.269295E-01,9.366731E-01,9.459482E-01,
    9.544675E-01,9.619834E-01,9.684390E-01,9.738289E-01,9.781519E-01,
    9.814106E-01,9.836669E-01,9.852081E-01,9.863813E-01,9.875357E-01,
    9.890228E-01,9.910811E-01,9.934913E-01,9.959172E-01,9.980205E-01,
    9.994608E-01,9.999930E-01,9.997557E-01,9.989839E-01,9.979123E-01,
    9.967737E-01,9.957356E-01,9.947115E-01,9.935534E-01,9.921156E-01,
    9.902549E-01,9.878596E-01,9.849324E-01,9.815036E-01,9.776035E-01,
    9.732611E-01,9.684764E-01,9.631369E-01,9.571062E-01,9.502540E-01,
    9.424569E-01,9.336897E-01,9.242893E-01,9.146707E-01,9.052333E-01,
    8.963613E-01,8.883069E-01,8.808462E-01,8.736445E-01,8.663755E-01,
    8.587203E-01,8.504295E-01,8.415047E-01,8.320109E-01,8.220154E-01,
    8.115868E-01,8.007874E-01,7.896515E-01,7.782053E-01,7.664733E-01,
    7.544785E-01,7.422473E-01,7.298229E-01,7.172525E-01,7.045818E-01,
    6.918553E-01,6.791009E-01,6.662846E-01,6.533595E-01,6.402807E-01,
    6.270066E-01,6.135148E-01,5.998494E-01,5.860682E-01,5.722261E-01,
    5.583746E-01,5.445535E-01,5.307673E-01,5.170130E-01,5.032889E-01,
    4.895950E-01,4.759442E-01,4.623958E-01,4.490154E-01,4.358622E-01,
    4.229897E-01,4.104152E-01,3.980356E-01,3.857300E-01,3.733907E-01,
    3.609245E-01,3.482860E-01,3.355702E-01,3.228963E-01,3.103704E-01,
    2.980865E-01,2.861160E-01,2.744822E-01,2.631953E-01,2.522628E-01,
    2.416902E-01,2.314809E-01,2.216378E-01,2.121622E-01,2.030542E-01,
    1.943124E-01,1.859227E-01,1.778274E-01,1.699654E-01,1.622841E-01,
    1.547397E-01,1.473081E-01,1.400169E-01,1.329013E-01,1.259913E-01,
    1.193120E-01,1.128820E-01,1.067113E-01,1.008052E-01,9.516653E-02,
    8.979594E-02,8.469044E-02,7.984009E-02,7.523372E-02,7.086061E-02,
    6.671045E-02,6.277360E-02,5.904179E-02,5.550703E-02,5.216139E-02,
    4.899699E-02,4.600578E-02,4.317885E-02,4.050755E-02,3.798376E-02,
    3.559982E-02,3.334856E-02,3.122332E-02,2.921780E-02,2.732601E-02,
    2.554223E-02,2.386121E-02,2.227859E-02,2.079020E-02,1.939185E-02,
    1.807939E-02,1.684817E-02,1.569188E-02,1.460446E-02,1.358062E-02,
    1.261573E-02,1.170696E-02,1.085608E-02,1.006476E-02,9.333376E-03,
    8.661284E-03,8.046048E-03,7.481130E-03,6.959987E-03,6.477070E-03,
    6.027677E-03,5.608169E-03,5.216691E-03,4.851785E-03,4.512008E-03,
    4.195941E-03,3.902057E-03,3.628371E-03,3.373005E-03,3.134315E-03,
    2.910864E-03,2.701528E-03,2.505796E-03,2.323231E-03,2.153333E-03,
    1.995557E-03,1.849316E-03,1.713976E-03,1.588899E-03,1.473453E-03,
    1.367022E-03,1.268954E-03,1.178421E-03,1.094644E-03,1.016943E-03,
    9.447269E-04,8.775171E-04,8.150438E-04,7.570755E-04,7.033755E-04,
    6.537050E-04,6.078048E-04,5.653435E-04,5.260046E-04,4.895061E-04,
    4.555970E-04,4.240548E-04,3.946860E-04,3.673178E-04,3.417941E-04,
    3.179738E-04,2.957441E-04,2.750558E-04,2.558640E-04,2.381142E-04,
    2.217445E-04,2.066711E-04,1.927474E-04,1.798315E-04,1.678023E-04,
    1.565566E-04,1.460168E-04,1.361535E-04,1.269451E-04,1.183671E-04,
    1.103928E-04,1.029908E-04,9.611836E-05,8.973323E-05,8.379694E-05,
    7.827442E-05,7.313312E-05,6.834142E-05,6.387035E-05,5.969389E-05,
    5.578862E-05,5.213509E-05,4.872179E-05,4.553845E-05,4.257443E-05,
    3.981884E-05,3.725877E-05,3.487467E-05,3.264765E-05,3.056140E-05,
    2.860175E-05,2.675841E-05,2.502943E-05,2.341373E-05,2.190914E-05,
    2.051259E-05,1.921902E-05,1.801796E-05,1.689899E-05,1.585309E-05,
    1.487243E-05,1.395085E-05,1.308528E-05,1.227327E-05,1.151233E-05,
    1.080001E-05,1.013364E-05,9.509919E-06,8.925630E-06,8.377852E-06,
    7.863920E-06,7.381539E-06,6.929096E-06,6.505136E-06,6.108221E-06,
    5.736935E-06,5.389831E-06,5.065269E-06,4.761667E-06,4.477561E-06,
    4.211597E-06,3.962457E-06,3.728674E-06,3.508881E-06,3.301868E-06,
    3.106561E-06,2.922119E-06,2.748208E-06,2.584560E-06,2.430867E-06,
    2.286786E-06,2.151905E-06,2.025656E-06,1.907464E-06,1.796794E-06,
    1.693147E-06,1.596032E-06,1.504903E-06,1.419245E-06,1.338600E-06,
    1.262556E-06,1.190771E-06,1.123031E-06,1.059151E-06,9.989507E-07,
    9.422514E-07,8.888804E-07,8.386690E-07,7.914539E-07,7.470770E-07,
    7.053860E-07
};

const float z_matching_f[] = {
    1.847260E-02,2.221101E-02,2.669819E-02,3.206937E-02,3.847832E-02,
    4.609784E-02,5.511953E-02,6.575257E-02,7.822113E-02,9.276013E-02,
    1.096090E-01,1.290077E-01,1.512047E-01,1.764441E-01,2.049517E-01,
    2.369246E-01,2.725123E-01,3.117820E-01,3.547064E-01,4.011473E-01,
    4.508369E-01,5.034164E-01,5.586361E-01,6.162734E-01,6.760982E-01,
    7.378822E-01,8.013019E-01,8.655573E-01,9.295791E-01,9.921293E-01,
    1.051821E+00,1.107509E+00,1.159527E+00,1.208869E+00,1.256834E+00,
    1.305008E+00,1.354758E+00,1.405594E+00,1.456414E+00,1.505960E+00,
    1.552826E+00,1.595902E+00,1.635768E+00,1.673573E+00,1.710604E+00,
    1.748280E+00,1.787504E+00,1.826609E+00,1.863108E+00,1.894332E+00,
    1.917479E+00,1.930529E+00,1.934819E+00,1.932650E+00,1.926395E+00,
    1.918437E+00,1.910430E+00,1.901224E+00,1.889000E+00,1.871996E+00,
    1.848545E+00,1.817792E+00,1.781627E+00,1.742514E+00,1.702749E+00,
    1.664439E+00,1.629207E+00,1.597360E+00,1.568896E+00,1.543823E+00,
    1.522157E+00,1.503611E+00,1.486673E+00,1.469595E+00,1.450709E+00,
    1.428440E+00,1.401587E+00,1.370094E+00,1.334220E+00,1.294275E+00,
    1.250610E+00,1.203696E+00,1.154316E+00,1.103284E+00,1.051347E+00,
    9.991789E-01,9.473958E-01,8.966222E-01,8.473981E-01,8.001576E-01,
    7.552379E-01,7.127879E-01,6.725198E-01,6.340976E-01,5.972433E-01,
    5.617313E-01,5.274921E-01,4.948809E-01,4.642586E-01,4.358841E-01,
    4.099313E-01,3.864261E-01,3.650566E-01,3.454812E-01,3.274095E-01,
    3.105939E-01,2.948102E-01,2.798194E-01,2.654100E-01,2.514084E-01,
    2.376753E-01,2.241211E-01,2.107484E-01,1.975839E-01,1.846574E-01,
    1.720018E-01,1.596918E-01,1.479415E-01,1.369428E-01,1.268279E-01,
    1.176796E-01,1.094970E-01,1.020943E-01,9.527993E-02,8.890075E-02,
    8.283548E-02,7.700982E-02,7.144001E-02,6.615436E-02,6.117199E-02,
    5.650407E-02,5.215121E-02,4.809566E-02,4.431720E-02,4.079734E-02,
    3.751912E-02,3.446846E-02,3.163764E-02,2.901901E-02,2.660364E-02,
    2.438164E-02,2.234097E-02,2.046415E-02,1.873456E-02,1.713788E-02,
    1.566174E-02,1.429644E-02,1.303702E-02,1.187897E-02,1.081725E-02,
    9.846470E-03,8.960687E-03,8.152811E-03,7.416025E-03,6.744115E-03,
    6.131421E-03,5.572778E-03,5.063463E-03,4.599169E-03,4.175971E-03,
    3.790291E-03,3.438952E-03,3.119341E-03,2.829038E-03,2.565722E-03,
    2.327186E-03,2.111280E-03,1.915766E-03,1.738589E-03,1.577920E-03,
    1.432128E-03,1.299781E-03,1.179667E-03,1.070694E-03,9.718623E-04,
    8.822531E-04,8.010231E-04,7.273884E-04,6.606347E-04,6.001146E-04,
    5.452416E-04,4.954847E-04,4.503642E-04,4.094455E-04,3.723345E-04,
    3.386739E-04,3.081396E-04,2.804370E-04,2.552996E-04,2.324859E-04,
    2.117772E-04,1.929758E-04,1.759024E-04,1.603947E-04,1.463059E-04,
    1.335031E-04,1.218660E-04,1.112857E-04,1.016634E-04,9.291003E-05,
    8.494468E-05,7.769425E-05,7.109247E-05,6.507936E-05,5.960061E-05,
    5.460706E-05,5.005417E-05,4.590157E-05,4.211268E-05,3.865437E-05,
    3.549661E-05,3.261220E-05,2.997643E-05,2.756693E-05,2.536339E-05,
    2.334738E-05,2.150221E-05,1.981268E-05,1.826500E-05,1.684667E-05,
    1.554631E-05,1.435360E-05,1.325915E-05,1.225443E-05,1.133169E-05,
    1.048387E-05,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,0.000000E+00,
    0.000000E+00
};

#define DENSITY_ALT_MIN   -2000.0
#define DENSITY_ALT_MAX    30000.0
#define DENSITY_ALT_COUNT  65

const float atmosphere_alt_density[] = {
    1.2067000, 1.1522000, 1.0996000, 1.0489000, 1.0000000, 0.9529000, 
    0.9075000, 0.8638000, 0.8217000, 0.7812000, 0.7423000, 0.7048000, 
    0.6689000, 0.6343000, 0.6012000, 0.5694000, 0.5389000, 0.5096000, 
    0.4817000, 0.4549000, 0.4292000, 0.4047000, 0.3813000, 0.3589000, 
    0.3376000, 0.3172000, 0.2978000, 0.2755000, 0.2546000, 0.2354000, 
    0.2176000, 0.2012000, 0.1860000, 0.1720000, 0.1590000, 0.1470000, 
    0.1359000, 0.1256000, 0.1162000, 0.1074000, 0.0993000, 0.0918200, 
    0.0848900, 0.0785000, 0.0725800, 0.0670457, 0.0618803, 0.0570847, 
    0.0526600, 0.0486014, 0.0448809, 0.0414650, 0.0383200, 0.0354153, 
    0.0327324, 0.0302558, 0.0279700, 0.0258602, 0.0239145, 0.0221215, 
    0.0204700, 0.0189482, 0.0175432, 0.0162416, 0.0150300
};

#define POLARIZABILITY_COUNT 23

const float air_polarizability_data[] = {
    1.77068737e-30,   1.76395511e-30,   1.75820723e-30,
    1.75325784e-30,   1.74896354e-30,   1.74521217e-30,
    1.74191484e-30,   1.73900037e-30,   1.73641110e-30,
    1.73409995e-30,   1.73202814e-30,   1.73016346e-30,
    1.72847899e-30,   1.72695208e-30,   1.72556354e-30,
    1.72429705e-30,   1.72313862e-30,   1.72207624e-30,
    1.72109952e-30,   1.72019945e-30,   1.71936817e-30,
    1.71859883e-30,   1.71788538e-30
};

#define SEA_LVL_AIR_DENSITY 1225.0        /* g/m^3         */
#define MOLAR_MASS_AIR      28.97         /* g/mol         */
#define MOLAR_FRACTION_N2   0.78084       /* dimensionless */
#define MOLAR_FRACTION_O2   0.20946       /* dimensionless */
#define POLARIZABILITY_N2   1.10e-31      /* m^3           */
#define POLARIZABILITY_O2   0.802e-31     /* m^3           */
// from http://arxiv.org/abs/0907.0782
// this is *extremely* sensitive
#define POLARIZABILITY_AIR  1.736549e-30  /* m^3           */
//                          sqrt(MOLAR_FRACTION_N2 * POLARIZABILITY_N2 * POLARIZABILITY_N2 + \
//                               MOLAR_FRACTION_O2 * POLARIZABILITY_O2 * POLARIZABILITY_O2)
#define AVAGADROS_NUMBER    6.0221413e23  /* dimensionless */
#define SOLAR_TEMPERATURE_K 5900.0        /* kelvins       */
#define SOLAR_HALF_ANGLE    0.00471500697 /* radians       */
// chosen so that the solar spectrum arriving from an area 
// the size of the sun has total power (area under spectrum) equal to 1:
#define BLACKBODY_NORMALIZING_CONSTANT 1.306097553259549e-12 /* dimensionless */
#define EARTH_RADIUS        6378100.0     /* meters        */

// todo: this duplicates the arrays in memory :(
Curve1f cie_xyz_x = Curve1f(SPECTRAL_COUNT, x_matching_f, Rect1f(SPECTRAL_MIN_NM, SPECTRAL_MAX_NM));
Curve1f cie_xyz_y = Curve1f(SPECTRAL_COUNT, y_matching_f, Rect1f(SPECTRAL_MIN_NM, SPECTRAL_MAX_NM));
Curve1f cie_xyz_z = Curve1f(SPECTRAL_COUNT, z_matching_f, Rect1f(SPECTRAL_MIN_NM, SPECTRAL_MAX_NM));
Curve1f air_density_alt_fraction = Curve1f(DENSITY_ALT_COUNT, atmosphere_alt_density, Rect1f(DENSITY_ALT_MIN, DENSITY_ALT_MAX));
Curve1f air_polarizability = Curve1f(POLARIZABILITY_COUNT, air_polarizability_data, Rect1f(SPECTRAL_MIN_NM, SPECTRAL_MAX_NM));

float rgb_to_xyz[] = { 
    0.49,    0.31,    0.20, 
    0.17697, 0.81240, 0.01063,
    0.0,     0.01,    0.99 
};

float xyz_to_srgb[] = {
     3.2404542, -1.5371385, -0.4985314,
    -0.9692660,  1.8760108,  0.0415560,
     0.0556434, -0.2040259,  1.0572252
};

SimpleMatrix<float,3,3> cie_rgb_to_xyz_mtx = SimpleMatrix<float,3,3>(rgb_to_xyz);
SimpleMatrix<float,3,3> xyz_to_srgb_mtx    = SimpleMatrix<float,3,3>(xyz_to_srgb);


//TODO: These should do smaerter resampling.
//      ...which should be added to Raster.
//      i.e. give Raster an explicit domain, and re-scale the sample
//      position to that domain.
//TODO: not energy preserving when spectrum is given.

template <typename T>
Vec<T,3> xyz_from_power_spectrum(const T *spectrum, size_t n, T min_wavelength_nm, T max_wavelength_nm) {
    Vec<T,3> out;
    Curve1f fns[3] = {cie_xyz_x, cie_xyz_y, cie_xyz_z};
    T step = (max_wavelength_nm - min_wavelength_nm) / n;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < n; j++) {
            T lambda = j * step + min_wavelength_nm;
            float wt = fns[i].sample<EDGE_CONSTANT,INTERP_CUBIC>(lambda);
            out[i] += wt * spectrum[j] * step;
        }
    }
    return out;
}

template <typename T>
Vec<T,3> xyz_from_power_spectrum(const T *spectrum, size_t n, T* wavelengths_nm) {
    Vec<T,3> out;
    Curve1f fns[3] = {cie_xyz_x, cie_xyz_y, cie_xyz_z};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < n; j++) {
            T lambda = wavelengths_nm[j];
            float wt = fns[i].sample<EDGE_CONSTANT,INTERP_CUBIC>(lambda);
            out[i] += wt * spectrum[j];
        }
        out[i] / n;
    }
    return out;
}

Vec3d spectral_line_1nm_to_xyz(double wavelength_nm, double intensity) {
    return Vec3d(
            cie_xyz_x.sample<EDGE_CONSTANT,INTERP_CUBIC>(wavelength_nm) * intensity,
            cie_xyz_y.sample<EDGE_CONSTANT,INTERP_CUBIC>(wavelength_nm) * intensity,
            cie_xyz_z.sample<EDGE_CONSTANT,INTERP_CUBIC>(wavelength_nm) * intensity
        );
}

double blackbody_spectral_radiance(double kelvins, double lambda) {
    double h = 6.62606957e-34; // planck's constant
    double c = 299792458.0;    // lightspeed
    double kb = 1.3806488e-23; // boltzmann constant
    
    lambda = lambda * 1e-9;    // to meters, from nm

    double a = 2*h*c*c / pow(lambda,5);
    double recip = exp(h*c / (lambda * kb * kelvins)) - 1;
    return BLACKBODY_NORMALIZING_CONSTANT * a / recip;
}

void blackbody_curve(double kelvins, double *curve, int n, double lambda_min, double lambda_max) {
    for (int i = 0; i < n; i++) {
        double lambda = (i / (n - 1.0)) * (lambda_max - lambda_min) + lambda_min;
        curve[i] = blackbody_spectral_radiance(kelvins, lambda);
    }
}

void spectral_rainbow(Raster<double,double,2,3> *img) {
    Vec2i dims = img->dataExtents();
    int img_w = dims[0];
    int img_h = dims[1];
    for (int i = 0; i < img_w; i++) {
        double lambda = (SPECTRAL_MAX_NM - SPECTRAL_MIN_NM) * (i / (double)img_w) + SPECTRAL_MIN_NM;
        Vec3d c_xyz = spectral_line_1nm_to_xyz(lambda, 0.9);
        Vec3d c_rgb = xyz_to_srgb_mtx * c_xyz;
        c_rgb = clamp<Vec3d>(c_rgb, 0.0, 10.0);
        for (double *p = c_rgb.begin(); p != c_rgb.end(); p++) {
            *p = std::pow(*p, 1/2.2);
        }
        for (int y = 0; y < img_h; y++) {
            img->set(Vec2i(i,y), c_rgb);
        }
    }
}

void temp_curve(Raster<double, double, 2, 3> *img) {
    Vec2i dims = img->dataExtents();
    int img_w = dims[0];
    int img_h = dims[1];
    int n = SPECTRAL_COUNT;
    double spectrum[n];
    for (int i = 0; i < img_w; i++) {
        double s = i / (double)img_w;
        double kelvins = s * 10000;
        blackbody_curve(kelvins, spectrum, n, SPECTRAL_MIN_NM, SPECTRAL_MAX_NM);
        Vec3d c_xyz = xyz_from_power_spectrum<double>(spectrum, n, SPECTRAL_MIN_NM, SPECTRAL_MAX_NM);
        Vec3d c_rgb = xyz_to_srgb_mtx * c_xyz;
        // gamma??
        for (double *p = c_rgb.begin(); p != c_rgb.end(); p++) { *p = std::pow(clamp(*p, 0.0, 10.0), 1/2.2); } 
        for (int y = 0; y < img_h; y++) {
            img->set(Vec2i(i,y), c_rgb);
        }
    }
}

//todo: make atmospheric_sample its own thing.
//  could cache these guys with a direct solar irradiance quantity to speed
//  things up. use ray differential to determine whether recomputation is necessary.
//  useful for single scatter. less so for multiple scatter.
//todo: make most of these properties spatially variable.
struct Atmosphere {
    double planet_r;
    double sea_level_density;
    double polarizing_coeff;
    double molar_mass_air;
    
    inline double alt(const Vec3d &p) const {
        Vec3d planet_ctr = Vec3d(0,0,-planet_r);
        return planet_ctr.dist(p) - planet_r;
    }
    
    inline double density(double alt) const {
        return sea_level_density * air_density_alt_fraction.sample<EDGE_CLAMP,INTERP_CUBIC>(alt);
    }
    
    inline double particle_density(double alt) const {
        return density(alt) * AVAGADROS_NUMBER / molar_mass_air;
    }
    
    // todo: this has a wavelength-dependent component.
    inline double polarizability(double lambda_m) const {
        //return POLARIZABILITY_AIR * 0.75;
        return polarizing_coeff * air_polarizability.sample<EDGE_CLAMP,INTERP_CUBIC>(lambda_m * 1e9);
    }
    
    bool raySegment(const Ray3d &r, Vec3d *p0, Vec3d *p1, bool *hitPlanet=NULL) const {
        double atmos_alt = air_density_alt_fraction.domain().max();
        Vec3d sph_ctr = Vec3d(0,0,-planet_r);
        double sph_r  = planet_r + atmos_alt;
        double s0, s1; // two roots.
        if (hitPlanet) *hitPlanet = false;
        if (trace_sphere(r, &s0, &s1, sph_ctr, sph_r)) {
            double ss0, ss1;
            if (s1 < 0) return false;
            s0 = max(0.0, s0);
            if (trace_sphere(r, &ss0, &ss1, sph_ctr, planet_r)) {
                if (ss0 < s1 and ss0 > 0) {
                    s1 = ss0;
                    if (hitPlanet) *hitPlanet = true;
                }
            }
            *p0 = r.atMultiple(s0);
            *p1 = r.atMultiple(s1);
            return true;
        } else {
            return false;
        }
    }
};

struct Sun {
    double half_radians;
    Vec3d dir; //normalized plz, kthx
    double temperature;
    
    Sun(double half_radians, Vec3d dir, double temperature):
        half_radians(half_radians),
        dir(dir),
        temperature(temperature) {}
    
    double getSample(double lambda, Vec3d *L, Random *rng=getRandom()) const {
        *L = Sampler<double>(rng).cap(dir, half_radians);
        return blackbody_spectral_radiance(temperature, lambda); // todo: could be a lut
    }
    
    bool intersects(const Vec3d &dir) const {
        return dir.dot(this->dir) <= cos(half_radians);
    }
};

/**
 * Extinction coefficient due to molecular effects.
 * 
 * @param lambda_m Wavelength of light in meters.
 * @param n_particles Number of particles per unit volume.
 * @param polarizability Average polarizability of particles.
 */
double rayleigh_molecular_ext_coeff(double lambda_m, double n_particles, double polarizability) {
    double l2 = lambda_m * lambda_m;
    return n_particles * 128 * pow(M_PI,5) * polarizability * polarizability /
           (3.0 * l2 * l2);
}

/**
 * Evaluate the Rayleigh phase function of light arriving along L viewed from I,
 * returning the fraction of light scattered from I to L.
 * 
 * @param I Normalized viewing direction.
 * @param L Normalized direction of illuminant photon.
 * @param lambda_m Wavelength of arriving photon, in meters.
 * @param n_particles Number of particles per unit volume.
 */
double rayleigh_molecular_phase(Vec3d I, Vec3d L, double lambda_m, double n_particles, double polarizability) {
    double cosa = I.dot(L);
    double cos2a = cosa * cosa;
    double l2 = lambda_m * lambda_m;
    double k = (1 + cos2a) * 8 * pow(M_PI,4) / (l2*l2);
    return k * polarizability * polarizability * n_particles;
}

/**
 * Attenuation of light along the line segment `(p0, p1)` due to Rayleigh effects
 * in the given atmosphere. 
 * 
 * @param lambda_m Wavelength of light to attenuate, in meters.
 * @param p0 Line segment begin.
 * @param p1 Line segment end.
 * @param atmos Atmospheric properties.
 * @return Total attenuation factor for the segment (1.0 is no attenuation).
 */
double atmospheric_segment_attenuation(
        double lambda_m, 
        const Vec3d &p0, const Vec3d &p1, 
        const Atmosphere &atmos) {
    Vec3d midpt    = (p0 + p1) / 2;
    double pathlen = p0.dist(p1);
    double alt = atmos.alt(midpt);
    double n_particles = atmos.particle_density(alt);
    double atten_k = rayleigh_molecular_ext_coeff(lambda_m, n_particles, atmos.polarizability(lambda_m));
    return exp(-pathlen * atten_k);
}

/** 
 * Fraction of light scattered toward `p0` from incoming direction `L` along 
 * the segment `(p0, p1)`. 
 * 
 * @param lambda_m Wavelength of light to scatter, in meters.
 * @param p0 Line segment begin.
 * @param p1 Line segment end.
 * @param L  Normalized light direction.
 * @param atmos Atmospheric properties.
 * @return Fraction of incoming light directed at `p0`.
 */
double atmospheric_segment_phase(
        double lambda_m, 
        const Vec3d &p0, const Vec3d &p1, Vec3d L,
        const Atmosphere &atmos) {
    Vec3d midpt = (p0 + p1) / 2;
    double pathlen = p0.dist(p1);
    double alt = atmos.alt(midpt);
    Vec3d I = (p0 - p1).unit();
    return pathlen * rayleigh_molecular_phase(I, L, lambda_m, atmos.particle_density(alt), atmos.polarizability(lambda_m));
}


double trace_atmospheric_attenuation(double lambda_m, const Ray3d &dir, double steps, const Atmosphere &atmos) {
    Vec3d p0, p1;
    bool hitPlanet = false;
    if (atmos.raySegment(dir, &p0, &p1, &hitPlanet)) {
        if (hitPlanet) return 0;
        Vec3d v = p1 - p0;
        double atten = 1;
        
        for (int i = 0; i < ceil(steps); i++) {
            double s0 =        i  / steps;
            double s1 = min((i+1) / steps, 1.0);
            Vec3d x0 = p0 + v * s0;
            Vec3d x1 = p0 + v * s1;
            atten *= atmospheric_segment_attenuation(lambda_m, x0, x1, atmos);
        }
        return atten;
    } else {
        // no intersection with atmosphere.
        return 1;
    }
}

double trace_atmospheric_scattering(double lambda_m, 
                                    const Ray3d &dir, 
                                    double steps, 
                                    const Atmosphere &atmos, 
                                    const Sun &sun, 
                                    Random *rng) {
    Vec3d p0, p1;
    if (atmos.raySegment(dir, &p0, &p1)) {
        Vec3d v = p1 - p0;
        double sct = 0;
        double running_atten = 1.0;
        Vec3d L;
        double irr = sun.getSample(lambda_m * 1e9, &L, rng); //could move this in the loop for better sampling.
        // todo: smaerter stepping
        for (int i = 0; i < steps; i++) {
            double s0 = i / steps;
            double s1 = min( (i+1) / steps, 1.0 );
            Vec3d x0 = p0 + v * s0;
            Vec3d x1 = p0 + v * s1;
            // light shadowing
            double irr_atten = trace_atmospheric_attenuation(lambda_m, Ray3d(x0, L), steps * 0.10, atmos);
            // single scatter from direct light
            double sct_amt = atmospheric_segment_phase(lambda_m, x0, x1, L, atmos);
            // incident light attenuated by shadowing, then by phase function,
            // then by the media between the detector and the current element
            sct += running_atten * sct_amt * irr_atten * irr;
            // compute additional attenuation of this element
            running_atten *= atmospheric_segment_attenuation(lambda_m, x0, x1, atmos);
        }
        return sct;
    } else {
        return 0; 
    }
}

void render_sky(Raster<double,double,2,3> *img) {
    int w = img->dataExtents()[0];
    int h = img->dataExtents()[1];
    
    Atmosphere atmos = {EARTH_RADIUS, 
                        SEA_LVL_AIR_DENSITY,
                        0.75, //1.102,
                        MOLAR_MASS_AIR};
    Sun sun(
        SOLAR_HALF_ANGLE,
        Vec3d(1,1,-0.02).unit(),
        SOLAR_TEMPERATURE_K);
    
    
    cout << "density: " << atmos.density(1) << endl;
    cout << "nparticles: " << atmos.particle_density(1) << endl;
    cout << "atten: " << rayleigh_molecular_ext_coeff(440*1e-9, atmos.particle_density(1), atmos.polarizability(550.0 * 1e-9)) << endl;
    
    double xwl[] = {440,550,680};
    for (int i = 0; i < 3; i++){
        double lam = xwl[i];
        cout << "atten(" << lam << "): ";
        lam *= 1e-9;
        cout << rayleigh_molecular_ext_coeff(lam, atmos.particle_density(1), atmos.polarizability(lam)) << endl;
    }
    
    //return;
    
    Vec2d img_ctr(0.5, 0.5); // not right, but whatevs.
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            Vec2d p_plane = 2 * (Vec2d(x,y) / w - img_ctr);
            double phi = M_PI * p_plane.mag() / 2;
            Vec3d p3d_plane = Vec3d(p_plane, 0).unit();
            Vec3d v = p3d_plane.rotate(p3d_plane ^ -Z_AXIS3d, phi - M_PI / 2).unit();
            Ray3d r = Ray3d(Vec3d(0,0,5000), v);
            
            const int spectrum_sz = 8;
            double wls[spectrum_sz];
            for (int i = 0; i < spectrum_sz; i++) {
                double s = i / (double)(spectrum_sz - 1);
                wls[i] = s * (660 - 420) + 420;
            }
            double spectrum[spectrum_sz] = {0};
            for (int i = 0; i < spectrum_sz; i++) {
                double lambda_m = wls[i] * 1e-9; // to meters
                //spectrum[i] = trace_atmospheric_attenuation(lambda_m, r, 500, atmos);
                spectrum[i] = 0.3 * trace_atmospheric_scattering(lambda_m, r, 50, atmos, sun, getRandom());
                //if (spectrum[i] != 1) cout << spectrum[i] << endl;
            }
            Vec3d c_xyz = xyz_from_power_spectrum(spectrum, spectrum_sz, wls);
            Vec3d c_rgb = xyz_to_srgb_mtx * c_xyz;
            for (double *p = c_rgb.begin(); p != c_rgb.end(); p++) { *p = std::pow(clamp(*p, 0.0, 10.0), 1/2.2); } 
            img->set(Vec2i(x,y), c_rgb);
            //img->set(Vec2i(x,y), x0.dist(x1) / 100000);
        }
        cout << (100*x/(double)w) << endl;
    }
}


int main(int argc, char** argv) {
    int img_w = 1024;
    int img_h = 1024;
    Raster<double,double,2,3> img(Vec2i(img_w, img_h));
    
    //temp_curve(&img);
    //save_png(img, "tempcurve2.png");
    //spectral_rainbow(&img);
    //save_png(img, "rainbow.png");
    render_sky(&img);
    save_png(img, "sky.png");
    
    double curve[SPECTRAL_COUNT];
    blackbody_curve(SOLAR_TEMPERATURE_K, curve, SPECTRAL_COUNT, SPECTRAL_MIN_NM, SPECTRAL_MAX_NM);
    double total = 0;
    for (int i = 0; i < SPECTRAL_COUNT; i++) {
        total += curve[i] * SPECTRAL_STEP_NM;
    }
    printf("%f\n", total);
    
    return 0;
}