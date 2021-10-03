function hRD = plugin_rd()
% A simple reference Design for the ZYBO Z7-20 Board.
% Provides a 720p@60 RGB video AXI4-stream source.
% Provides a 720p@60 RGB video AXI4-stream sink.

% Construct reference design object
hRD = hdlcoder.ReferenceDesign('SynthesisTool', 'Xilinx Vivado');

% Basic settings
hRD.ReferenceDesignName = 'Einfaches AXI4s-Video (RGB)';
hRD.BoardName = 'ZYBO Z7-20';
hRD.SupportedToolVersion = {'2020.1'};
hRD.AddJTAGMATLABasAXIMasterParameter = 'false';

% register custom constraints + files/ip
hRD.CustomConstraints = {'axi4s_rgb_rd.xdc'};
hRD.CustomFiles = {'ipcore'};

% add custom Vivado design
hRD.addCustomVivadoDesign( ...
    'CustomBlockDesignTcl', 'reference_design.tcl', ...
    'VivadoBoardPart','digilentinc.com:zybo-z7-20:part0:1.0');

%% Add interfaces
% add clock interface
hRD.addClockInterface( ...
    'ClockConnection', 'dvi2rgb/PixelClk', ...
    'ResetConnection', 'axi_reset_const/dout');

hRD.addAXI4StreamVideoInterface(...
    'MasterChannelConnection', 'stream2rgb/video_in', ...   %Masterport connection IP-Core
    'MasterChannelDataWidth', 24, ...                       %width of TDATA @ masterport
    'SlaveChannelConnection','rgb2stream/video_out', ...    %Slaveport connection IP-Core
    'SlaveChannelDataWidth', 24, ...                        %width of TDATA on slaveport
    'InterfaceID', 'AXI4-Stream RGB Video');                %Label for Workflow Advisor

%ANMERKUNG: Obwohl lt. Handbuch ein AXI4-Slave Interf. optional sein sollte
%schlägt die Validierung des ReferenceDesign-Objekts ohne dieses fehl...
hRD.addAXI4SlaveInterface( ...
    'InterfaceConnection', 'axi_interconnect_0/M00_AXI', ...
    'BaseAddress',         '0x40010000', ...
    'MasterAddressSpace',  'zynq_ps/Data');
