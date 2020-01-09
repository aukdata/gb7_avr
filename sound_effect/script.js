
function playNote(freq, length)
{
    const context = new AudioContext();
    context.sampleRate = freq * 100;
    const buf = context.createBuffer(1, freq * 100 * length, freq * 100);
    const data = buf.getChannelData(0);

    for (let i = 0; i < data.length; i++)
    {
        if ((i % 100) < 50)
        {
            data[i] = 0.5;
        }
        else
        {
            data[i] = -0.5;
        }
    }
    
    const src = context.createBufferSource();
    src.buffer = buf;
    src.connect(context.destination);
    src.start();
}

function playNotes(notes)
{
    const dict = {
        C : 1e6 / 3822,
        Cs: 1e6 / 3677,
        D : 1e6 / 3405,
        Ds: 1e6 / 3214,
        E : 1e6 / 3033,
        F : 1e6 / 2863,
        Fs: 1e6 / 2702,
        G : 1e6 / 2551,
        Gs: 1e6 / 2407,
        A : 1e6 / 2272,
        As: 1e6 / 2145,
        B : 1e6 / 2024,
        Ch: 1e6 / 1911,
    };

    let accum = 0;
    for (const note of notes)
    {
        if (note.tone !== 'None')
        {
            setTimeout(() => {
                playNote(dict[note.tone], note.length);
            }, accum * 1000);
        }
        accum += note.length;
    }
}

function convertToNotes(text)
{
    const lines = text.split('\n');
    return lines.filter(line => !line.match(/^ *$/)).map(line => {
        const res = line.match(/^(None|C|Cs|D|Ds|E|F|Fs|G|Gs|A|As|B|Ch) *: *(\d*\.?\d*)$/i);
        if (res)
        {
            return {
                tone: res[1],
                length: Number(res[2]),
            };
        }
        throw 'Syntax error: ' + line;
    });
}

document.addEventListener('load', e => {
    const textarea = document.getElementById('notes');
    const result = document.getElementById('result');

    document.getElementById('play').addEventListener('click', e => {
        try
        {
            const notes = convertToNotes(textarea.value);
            playNotes(notes);
        }
        catch (mes)
        {
            result.value = mes;
        }
    }, true);

    document.getElementById('convert').addEventListener('click', e => {
        try
        {
            const notes = convertToNotes(textarea.value);
            result.value = notes.map(note => 'speaker::enqueue_note(Tone::' + note.tone + ', ' + note.length + 'e6);\n').join('');
            result.focus();
            result.select();
        }
        catch (mes)
        {
            result.value = mes;
        }
    }, true);
}, true);
