function [rd, boardName] = hdlcoder_ref_design_customization
% Reference design plugin registration file

rd = {'ZyboZ720.hello_world_rd.plugin_rd',...
    'ZyboZ720.simple_axis_reference.plugin_rd'};
boardName = 'ZYBO Z7-20';

end