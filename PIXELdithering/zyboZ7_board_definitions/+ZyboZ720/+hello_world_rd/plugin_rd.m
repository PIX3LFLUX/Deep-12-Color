function hRD = plugin_rd()
% Reference design definition

% Construct reference design object
hRD = hdlcoder.ReferenceDesign('SynthesisTool', 'Xilinx Vivado');

hRD.ReferenceDesignName = 'Minimales HelloWorld';
hRD.BoardName = 'ZYBO Z7-20';

% Tool information
% It is recommended to use a tool version that is compatible with the supported tool
% version. If you choose a different tool version, it is possible that HDL Coder is
% unable to create the reference design project for IP core integration.
hRD.SupportedToolVersion = {'2020.1'};

%% Add custom design files
% add custom Vivado design
%hRD.addCustomVivadoDesign( ...
%    'CustomBlockDesignTcl', 'reference_design.tcl');

% add custom Vivado design
hRD.addCustomVivadoDesign( ...
    'CustomBlockDesignTcl', 'reference_design.tcl', ...
    'VivadoBoardPart','digilentinc.com:zybo-z7-20:part0:1.0');

%hRD.CustomFiles = {'ZYBO_zynq_def.xml'};

%% Add interfaces
% add clock interface
hRD.addClockInterface( ...
    'ClockConnection',   'zynq_ps/FCLK_CLK1', ...
    'ResetConnection', 'rst_gen/peripheral_aresetn');

hRD.addAXI4SlaveInterface( ...
    'InterfaceConnection', 'axi_interconnect_0/M00_AXI', ...
    'BaseAddress',         '0x40010000', ...
    'MasterAddressSpace',  'zynq_ps/Data');
