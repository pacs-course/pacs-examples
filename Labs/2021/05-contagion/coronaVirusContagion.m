% Source: https://github.com/seismotologist/coronaVirusContagion
%
% Toy model for contagion of Corona virus, and how Social Distancing (SD)
% affects its spread, inspired by models shown in Washington Post article 
% by Harry Stevens, washingtonpost.com/graphics/2020/world/corona-simulator
% 
% The model can be used to show how meeting points such as pubs
% can undo the gains made from SD practices. That is, it is crucial to 
% minimise the number of trips to such places, and to be extra careful on 
% the trips we do have to make.
%
% by Men-Andrin Meier, v1.0, 20/04/2020
% mmeier@caltech.edu 
% @seismotologist
% www.seismotology.com
%
% Feel free to use and modify the model. Please acknowledge 
% github.com/seismotologist & @seismotologist. Thank you (\_/)
% -------------------------------------------------------------------------

clear all 

% DESCRIPTION: A fraction of people in simulation ('agents') move around,
% while others practice Social Distancing (SD) and stay home. At home,
% people do not get infected by passing people who carry the virus, but 
% they can get infected at the pub. Every agent has to go to the pub 
% at a regular time interval. 

% Model parameters
nagents      = 200;                % No. of agents
tstepsperday = 5;                  % No. of time steps per day (affects display only)
nt           = 150;                % No. of time steps of simulation
fractSD      = .7;                 % Fraction of people who practice social distancing 
dtGoToPub    = 7*tstepsperday;     % Every <dtGoToPub> time steps, people go to the pub
dtAtPub      = 1;                  % No. of time steps spent at pub
dtHeal       = 10*tstepsperday;    % Time after which peple recover, and become immune
rcont        = .04;                % Distance below which agents pass on disease
dr           = .02;                % Step length per time step
pubsize      = .1;                 % Pub size
xpub         = .5;                 % Pub location
ypub         = .5; 

% Display parameters
verbose      = 0;
dtpause      = .01;    % simulation speed
mkSize       = 6;
writeGif     = 1;
dtgif        = .2;
gifName      = sprintf('contagion_pub_dtGoToPub%i_dtHeal%i.gif',dtGoToPub,dtHeal);



% Pseudo-code -----------------------------------------------------
% for it = 1:nt
%     for ia = 1:nagents
%         % 1. Movement (take step/ stay put / go to pub / ... )
%         % 2. Health   (pass to others / heal)
%     end
%     % Plot
% end -------------------------------------------------------------

tString = {sprintf('Time between trips to pub: %i  days',dtGoToPub/tstepsperday); ...
           sprintf('Fraction of people who do Social Distancing: %i%%',fractSD*100); ...
           sprintf('Time sick: %i days',dtHeal/tstepsperday)};

rng(55)

% Domain is from 0 to 1 in x and y directions; random initial positions and
% directions
x     = rand(nagents,1);
y     = rand(nagents,1);
alpha = 2*pi*rand(nagents,1);   
dx    = dr*sin(alpha);
dy    = dr*cos(alpha);

% Who does Social Distancing (SD)? 
doesSD = false(nagents,1);
doesSD(rand(nagents,1)>(1-fractSD)) = true; 

% Health status
isVulnerable  = true (nagents,1);
isSick        = false(nagents,1);
isImmune      = false(nagents,1);
tSick         = zeros(nagents,1);
iseed         = 1;
isSick(iseed) = true;
doesSD(iseed) = false;

% Pub
tGoToPub  = randi(dtGoToPub,nagents,1);   % time until next pub trip 
tAtPub    = zeros(nagents,1);
isAtPub   = false(nagents,1);
stayAtPub = false(nagents,1);

% Initialise figure
hf = figure(445); clf;
s1 = subplot(8,5, 6:9); hold on; box on;
set(gca,'xlim',[0 nt/tstepsperday],'ylim',[0 nagents])
title(tString,'fontWeight','normal')
xlabel('Days')
ylabel('No. of people')

s2 = subplot(8,5,16:40); hold on; box on;
set(gca,'xlim',[0 1],'ylim',[0 1])
set(gca,'xtick',[],'ytick',[])
rectangle('Position',[xpub ypub pubsize pubsize],'faceColor',[.7 .7 .7])

csick       = [160,82 , 45]/256; % Colours
cvulnerable = [173,216,230]/256;
cimmune     = [221,160,221]/256;

% Initial positions
pa1 = plot(x(isVulnerable), y(isVulnerable),'sk','markerFaceColor',cvulnerable,'markerSize',mkSize);
pa2 = plot(x(isSick),       y(isSick),      'sk','markerFaceColor',csick      ,'markerSize',mkSize);
pa3 = plot(x(isImmune),     y(isImmune),    'sk','markerFaceColor',cimmune    ,'markerSize',mkSize);
pa4 = [];
pa5 = [];

xbak = x;
ybak = y;

nvulnerable = [];
nsick       = [];
nimmune     = [];
tt          = [];


% Loop over all time steps
for it = 1:nt
    
    % Loop over all agents
    for ia = 1:nagents

	
        % Movement
        % ========
        % Check next action of agent
        
        tGoToPub(ia) = tGoToPub(ia)-1;                               % Update time to pub 
        
        if isAtPub(ia); tAtPub   (ia) = tAtPub(ia)+1;                % Update time at pub
                           stayAtPub(ia) = tAtPub(ia)<dtAtPub;
        else               xbak(ia) = x(ia);                         % Backup position
                           ybak(ia) = y(ia);
        end
        
        clear nextMove                                                                  % ACTIONs
        if tGoToPub(ia) >0 & ~doesSD(ia);               nextMove = 'takeStep'; end      % Keep walking 
        if tGoToPub(ia) >0 &  doesSD(ia);               nextMove = 'stayPut'; end       % Stay at home
        if isAtPub(ia)     & tAtPub(ia) <dtAtPub; nextMove = 'stayPut'; end             % Stay at pub
        if isAtPub(ia)     & tAtPub(ia)==dtAtPub; nextMove = 'returnFromPub'; end       % Go back to where you were before pub
        if tGoToPub(ia)==0                              nextMove = 'goToPub'; end       % Go to pub
        
        if verbose; fprintf(1,sprintf('Next action of agent %i at (%3.1f/%3.1f): %s\n',ia,x(ia),y(ia),nextMove)); end

        if strcmp(nextMove,'goToPub')
            
            % Jump to pub and stay for <tGoToPub> time steps
            x(ia)           = xpub+rand(1)*pubsize;
            y(ia)           = ypub+rand(1)*pubsize;
            tGoToPub(ia) = -1;
            isAtPub(ia)  = true;
            
        elseif strcmp(nextMove,'returnFromPub')
            
            % Return to position from before pub, and re-set pub clocks
            x(ia)        = xbak(ia);
            y(ia)        = ybak(ia);
            tGoToPub(ia) = dtGoToPub;
            tAtPub(ia)   = 0;
            isAtPub(ia)  = false;
            
        elseif strcmp(nextMove,'stayPut')
        
            % Do nothing
            pi-3.141592653589793;
            
        elseif strcmp(nextMove,'takeStep')
        
            % Try new position
            xnew = x(ia) + dx(ia);
            ynew = y(ia) + dy(ia);
            
            % If you hit a wall, try a random new direction, until you no
            % longer hit a wall
            hitWall = xnew>1 | xnew<0 | ynew>1 | ynew<0;
            ntries  = 0;
            stillTrying = true;
            while hitWall & stillTrying
                
                % Choose random new direction & update position
                alpha(ia) = 2*pi*rand(1,1);
                dx(ia)    = dr*sin(alpha(ia));
                dy(ia)    = dr*cos(alpha(ia));
                xnew      = x(ia) + dx(ia);
                ynew      = y(ia) + dy(ia);
                hitWall = xnew>1 | xnew<0 | ynew>1 | ynew<0;
                
                ntries = ntries+1;
                if ntries>100; stillTrying = false; end
            end
            x(ia) = xnew;
            y(ia) = ynew;
        end
        
        
        
        % Health status
        % =============
        % Check if agent heals and becomes immune
        if isSick(ia)
            tSick(ia) = tSick(ia)+1;
            if tSick(ia)>dtHeal
                isSick(ia)   = false;
                isImmune(ia) = true;
            end
        end
        
        
        % Contagion
        if isSick(ia)
        
            % Pass on disease to all within <rcont> that are not immune
            xdist      = x(ia) - x;
            ydist      = y(ia) - y;
            r          = sqrt(xdist.^2+ydist.^2);   % Distance to other agent
            meetAgents = r<=rcont;
            isHome     = x==xbak & y==ybak;         % People at home don't get infected
            isSick(meetAgents & ~isImmune & ~isHome) = true;
        end
    end
    
    % Count cases
    isVulnerable = ~isSick & ~isImmune;
    nvulnerable  = [nvulnerable, sum(isVulnerable)];
    nsick        = [nsick,       sum(isSick)];
    nimmune      = [nimmune,     sum(isImmune)];
    tt           = [tt,          it/tstepsperday];
    nall         = nvulnerable + nsick + nimmune;
    if nall~=nagents; error('Counts dont add up.'); end
    
    
    % Update figure
    subplot(s2)
    delete([pa1;pa2;pa3;pa4;pa5])
    
    pa1 = plot(x(isVulnerable), y(isVulnerable),'ok','markerFaceColor',cvulnerable,'markerSize',mkSize);
    pa2 = plot(x(isSick),       y( isSick),     'ok','markerFaceColor',csick      ,'markerSize',mkSize);
    pa3 = plot(x(isImmune),     y( isImmune),   'ok','markerFaceColor',cimmune    ,'markerSize',mkSize);
    
    pa4 = plot([x(isAtPub),xbak(isAtPub,:)]', [y(isAtPub),ybak(isAtPub,:)]' ,'-','color',[.8 .8 .8]);
    pa5 = plot(xbak(isAtPub), ybak(isAtPub) ,'ok');
    
    title(sprintf('t = %3.1f days',it/tstepsperday))
    pause(dtpause)
    
    subplot(s1); hold on;
    pnv = plot(tt,nvulnerable,'-','lineWidth',2,'color',cvulnerable);
    pns = plot(tt,nsick,      '-','lineWidth',2,'color',csick);
    pni = plot(tt,nimmune,    '-','lineWidth',2,'color',cimmune);
    lg  = legend([pnv,pns,pni],'Healthy','Sick','Immune');
    set(lg,'location','southEastOutside')

    
    if writeGif
        
        % Capture the plot as an image & write to gif
        drawnow
        frame      = getframe(hf);
        im         = frame2im(frame);
        [imind,cm] = rgb2ind(im,256);
        if it==1; imwrite(imind,cm,gifName,'gif','DelayTime',dtgif, 'Loopcount',inf);
        else      imwrite(imind,cm,gifName,'gif','DelayTime',dtgif,'WriteMode','append');
        end
    end
end
