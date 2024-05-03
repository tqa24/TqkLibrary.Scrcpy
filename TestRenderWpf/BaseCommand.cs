using System;
using System.Windows.Input;

namespace TestRenderWpf
{
    public class BaseCommand : ICommand
    {
        readonly Action _action;
        public BaseCommand(Action action)
        {
            _action = action;
        }

        public event EventHandler? CanExecuteChanged;

        public bool CanExecute(object? parameter)
        {
            return true;
        }

        public void Execute(object? parameter)
        {
            _action();
        }
    }
}
