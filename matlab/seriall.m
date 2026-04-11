clear
clc
close all
%a=zeros(1023,1);
%i=0;
s = serialport('COM8', 115200); 
configureTerminator(s, 'LF');  % Set the terminator to LF (line feed)
filepath = fullfile('C:', 'Users', 'Taha Barazy', 'Documents', 'received_data.txt');
flush(s);
file = fopen(filepath,'w');
while 1
    if s.NumBytesAvailable > 0
%         date = read(s,4,'int32')
        data = readline(s);
        elements = strsplit(data);
        
        % Convert each element to an integer
        counter = str2double(elements{1});
        input = str2double(elements{2});
        output=str2double(elements{3});
        error = str2double(elements{4});
 
        file = fopen(filepath,'a');
        fprintf(file, '%d %d %d %d\n', counter,input,output,error);
        fclose(file);
    end
end

%%
clc
clear all
close all